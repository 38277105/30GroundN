#ifndef ELLIPSOIDMAG_H
#define ELLIPSOIDMAG_H

#include "Eigen/Eigen"
#include "Eigen/Dense"
#include <QFile>
class EllipsoidMag
{
public:
    EllipsoidMag();
    EllipsoidMag(int id);
    ~EllipsoidMag();
    void Mag_Fit(const float x[], const float y[], const float z[], int size, float b[]);
private:
    void Ellipsoid_Fit(const Eigen::Matrix<double, Eigen::Dynamic, 3>& data,Eigen::Vector3d&center, Eigen::Vector3d& radii,Eigen::Matrix3d& evecs,float b[]);
private:
    QFile inFile,outFile;

};

#endif // ELLIPSOIDMAG_H
