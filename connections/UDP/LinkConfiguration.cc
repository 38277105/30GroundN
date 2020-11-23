/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009, 2015 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

    QGROUNDCONTROL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QGROUNDCONTROL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/*!
    @file
       @brief Link specific configuration base class
       @author Gus Grubba <mavlink@grubba.com>
*/

#include "LinkConfiguration.h"
#include "UDPLink.h"
#include "LinkInterface.h"


#define LINK_SETTING_ROOT "LinkConfigurations"

LinkConfiguration::LinkConfiguration(const QString& name)
    : _link(NULL)
    , _name(name)
    , _dynamic(false)
    , _autoConnect(false)
{
    _name = name;
    if (_name.isEmpty()) {
        qWarning() << "Internal error";
    }
}

LinkConfiguration::LinkConfiguration(LinkConfiguration* copy)
{
    _link       = copy->link();
    _name       = copy->name();
    _dynamic    = copy->isDynamic();
    _autoConnect= copy->isAutoConnect();
    Q_ASSERT(!_name.isEmpty());
}

void LinkConfiguration::copyFrom(LinkConfiguration* source)
{
    Q_ASSERT(source != NULL);
    _link       = source->link();
    _name       = source->name();
    _dynamic    = source->isDynamic();
    _autoConnect= source->isAutoConnect();
}

/*!
  Where the settings are saved
  @return The root path of the setting.
*/
const QString LinkConfiguration::settingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

/*!
  Configuration Factory
  @return A new instance of the given type
*/
LinkConfiguration* LinkConfiguration::createSettings(int type, const QString& name)
{
    LinkConfiguration* config = NULL;
    switch(type) {

        case LinkConfiguration::TypeUdp:
            config = new UDPConfiguration(name);
            break;

    }
    return config;
}

/*!
  Duplicate link settings
  @return A new copy of the given settings instance
*/
LinkConfiguration* LinkConfiguration::duplicateSettings(LinkConfiguration* source)
{
    LinkConfiguration* dupe = NULL;
    switch(source->type()) {

        case TypeUdp:
            dupe = new UDPConfiguration(dynamic_cast<UDPConfiguration*>(source));
            break;

        default:
            break;
    }
    return dupe;
}

void LinkConfiguration::setName(const QString name)
{
    _name = name;
    emit nameChanged(name);
}

void LinkConfiguration::setLink(LinkInterface* link)
{
    _link = link;
    emit linkChanged(link);
}