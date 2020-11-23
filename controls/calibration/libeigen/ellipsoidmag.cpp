#include "ellipsoidmag.h"
#include <QCoreApplication>
#include <QDir>

using namespace Eigen;
EllipsoidMag::EllipsoidMag()
{
}
EllipsoidMag::EllipsoidMag(int id)
{
    QString basePath=QCoreApplication::applicationDirPath()+"/data/mag/";
    QDir dr(basePath);
    if(!dr.exists())
        dr.mkdir(basePath);
    QString str=QString("inMagData%1.csv").arg(id);
    inFile.setFileName(basePath+str);
    bool ret=inFile.open(QFile::WriteOnly);
    str=QString("outMagParam%1.csv").arg(id);
    outFile.setFileName(basePath+str);
    ret=outFile.open(QFile::WriteOnly);
}
EllipsoidMag::~EllipsoidMag()
{
    if(inFile.isOpen())
        inFile.close();
    if(outFile.isOpen())
        outFile.close();
}

void EllipsoidMag::Mag_Fit(const float x[], const float y[], const float z[],int size, float b[])
{
    Eigen::Matrix<double, Eigen::Dynamic, 3> magData;
    magData.resize(size,3);
    for(int i=0;i<size;i++)
    {
        Eigen::Vector3d point(x[i]/1000.0,y[i]/1000.0,z[i]/1000.0);
        magData.row(i)=point.transpose();
    }

    Eigen::Vector3d center;
    Eigen::Vector3d radii;
    Eigen::Matrix3d tr;
    float c[9];
    Ellipsoid_Fit(magData,center,radii,tr,c);      //第一次计算，输出椭球的基本参数，用于剔除噪声点

    double r = radii(0)*radii(1)*radii(2);
    r = pow(r,1/3.0);
    double* err=new double[size];
    Eigen::Matrix<double, Eigen::Dynamic, 3> newData;
    newData.resize(size,3);
    int dNum=0;
    for(int i = 0;i<magData.rows();i++)
    {
        Eigen::Vector3d point = magData.row(i);
        Eigen::Vector3d pointCentered = point-center;
        Eigen::Vector3d pointCaled = tr*pointCentered;
        double dis = sqrtf(pointCaled(0)*pointCaled(0)+pointCaled(1)*pointCaled(1)+pointCaled(2)*pointCaled(2));
        err[i] = sqrtf((r-dis)*(r-dis));
        if(err[i]<0.025)                                  //去掉噪声点
            newData.row(dNum++)=point.transpose();
    }

    magData.resize(dNum,3);
    magData=newData.topRows(dNum);
    Ellipsoid_Fit(magData,center,radii,tr,b);            //把去掉噪声点数据重新计算一次

    //文件记录
    if(inFile.isOpen())
    {
        QString str;
        for(int i=0;i<size;i++)
        {
            str=QString("%1,%2,%3,%4\r\n").arg(x[i]/1000.0,0,'f',4).arg(y[i]/1000.0,0,'f',4)
                    .arg(z[i]/1000.0,0,'f',4).arg(err[i],0,'f',8);
            inFile.write(str.toStdString().c_str());
        }
    }
    delete []err;
    if(outFile.isOpen())
    {
        QString str,str1;
        for(int i=0;i<9;i++)
        {
            if(i==0)
                str=QString::number(b[i],'g',10);
            else
                str+=","+QString::number(b[i],'g',10);

            if(i==0)
                str1=QString::number(c[i],'g',10);
            else
                str1+=","+QString::number(c[i],'g',10);
        }
        str1+=QString(",%1\r\n").arg(size);
        outFile.write(str1.toStdString().c_str());
        str+=QString(",%1\r\n").arg(dNum);;
        outFile.write(str.toStdString().c_str());
    }
}

void EllipsoidMag::Ellipsoid_Fit(const Eigen::Matrix<double, Eigen::Dynamic, 3>& data,Eigen::Vector3d&center, Eigen::Vector3d& radii,Eigen::Matrix3d& tr,float b[])
{
        const auto& x = data.col(0);
        const auto& y = data.col(1);
        const auto& z = data.col(2);

        auto x_sq = x.cwiseProduct(x).eval();
        auto y_sq = y.cwiseProduct(y).eval();
        auto z_sq = z.cwiseProduct(z).eval();

        /*
         * fit ellipsoid in the form Ax^2 + By^2 + Cz^2 + 2Dxy + 2Exz + 2Fyz + 2Gx +
         * 2Hy + 2Iz + J = 0 and A + B + C = 3 constraint removing one extra
         * parameter
         */
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> D;

        D.resize(data.rows(), 9);
        D.col(0) = x_sq + y_sq - 2. * z_sq;
        D.col(1) = x_sq + z_sq - 2. * y_sq;
        D.col(2) = 2. * x.cwiseProduct(y);
        D.col(3) = 2. * x.cwiseProduct(z);
        D.col(4) = 2. * y.cwiseProduct(z);
        D.col(5) = 2. * x;
        D.col(6) = 2. * y;
        D.col(7) = 2. * z;
        D.col(8).setOnes();


        // solve the normal system of equations
        auto d2 = (x_sq + y_sq + z_sq).eval(); // the RHS of the llsq problem (y's)
        auto u = (D.transpose() * D).bdcSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(D.transpose() * d2).eval(); // solution to the normal equations

        /*
         * find the ellipsoid parameters
         * convert back to the conventional algebraic form
         */
        Eigen::Matrix<double, 10, 1> v;

        v(0) = u(0) + u(1) - 1.;
        v(1) = u(0) - 2. * u(1) - 1.;
        v(2) = u(1) - 2. * u(0) - 1.;
        v.segment<7>(3) = u.segment<7>(2);


        // form the algebraic form of the ellipsoid
        Eigen::Matrix4d A;
        A <<v(0), v(3), v(4), v(6),
            v(3), v(1), v(5), v(7),
            v(4), v(5), v(2), v(8),
            v(6), v(7), v(8), v(9);

        // find the center of the ellipsoid
        center = -A.block<3,3>(0,0).bdcSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(v.segment<3>(6));
        // form the corresponding translation matrix
        Eigen::Matrix4d T(Eigen::Matrix4d::Identity());
        T.block<1,3>(3,0) = center.transpose();
        // translate to the center
        auto R = (T * A * T.transpose()).eval();
        // solve the eigenproblem
        Eigen::EigenSolver<Eigen::Matrix3d> solver(R.block<3,3>(0,0) / -R(3,3));
        radii = solver.eigenvalues().cwiseAbs().cwiseInverse().cwiseSqrt();
        MatrixXcd evecc = solver.eigenvectors();
        MatrixX3d evecs = evecc.real().transpose();
        double r = radii(0)*radii(1)*radii(2);
        r = pow(r,1/3.0);
        //qDebug()<<"r: "<<r;
        Matrix3d matD;
        matD <<  r/radii(0),0,0,
                0,r/radii(1),0,
                0,0,r/radii(2);
        tr = evecs*matD*(evecs.transpose());
        for (size_t i = 0; i < 3; ++i) {
            if(solver.eigenvalues()(i).real() < 0.) {
                radii(i) = -radii(i);
            }
        }

        //填充磁的九个参数值
        int num=0;
        for(int i=0;i<3;i++)             //取矩阵的上三角给B0-B5
        {
            for(int j=i;j<3;j++)
            {
                b[num]=tr(i,j);
                num++;
            }
        }
        for(int i=0;i<3;i++)         //取偏移量(center)给B6-B8
        {
            b[num]=center(i);
            num++;
        }
}
