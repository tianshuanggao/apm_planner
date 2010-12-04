///*=====================================================================
//
//QGroundControl Open Source Ground Control Station
//
//(c) 2009, 2010 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
//
//This file is part of the QGROUNDCONTROL project
//
//    QGROUNDCONTROL is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    QGROUNDCONTROL is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.
//
//======================================================================*/

/**
 * @file
 *   @brief Definition of the class Pixhawk3DWidget.
 *
 *   @author Lionel Heng <hengli@student.ethz.ch>
 *
 */

#ifndef PIXHAWK3DWIDGET_H
#define PIXHAWK3DWIDGET_H

#include <osgText/Text>

#include "Imagery.h"
#include "ImageWindowGeode.h"

#ifdef QGC_LIBFREENECT_ENABLED
#include "Freenect.h"
#endif

#include "Q3DWidget.h"

class UASInterface;

/**
 * @brief A 3D View widget which displays vehicle-centric information.
 **/
class Pixhawk3DWidget : public Q3DWidget
{
    Q_OBJECT

public:
    explicit Pixhawk3DWidget(QWidget* parent = 0);
    ~Pixhawk3DWidget();

public slots:
    void setActiveUAS(UASInterface* uas);

private slots:
    void showGrid(int state);
    void showTrail(int state);
    void showWaypoints(int state);
    void selectMapSource(int index);
    void selectVehicleModel(int index);
    void recenter(void);
    void toggleFollowCamera(int state);

    void insertWaypoint(void);
    void moveWaypoint(void);
    void setWaypoint(void);
    void deleteWaypoint(void);
    void setWaypointAltitude(void);
    void clearAllWaypoints(void);

protected:
    QVector< osg::ref_ptr<osg::Node> > findVehicleModels(void);
    void buildLayout(void);
    virtual void display(void);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

    UASInterface* uas;

private:
    osg::ref_ptr<osg::Geode> createGrid(void);
    osg::ref_ptr<osg::Geode> createTrail(void);
    osg::ref_ptr<Imagery> createMap(void);
    osg::ref_ptr<osg::Node> createTarget(void);
    osg::ref_ptr<osg::Group> createWaypoints(void);
    osg::ref_ptr<osg::Geode> createRGBD3D(void);

    void setupHUD(void);
    void resizeHUD(void);

    void updateHUD(double robotX, double robotY, double robotZ,
                   double robotRoll, double robotPitch, double robotYaw);
    void updateTrail(double robotX, double robotY, double robotZ);
    void updateImagery(double originX, double originY, const QString& zone);
    void updateTarget(void);
    void updateWaypoints(void);
#ifdef QGC_LIBFREENECT_ENABLED
    void updateRGBD(void);
#endif

    void markTarget(void);

    int findWaypoint(int mouseX, int mouseY);
    void showInsertWaypointMenu(const QPoint& cursorPos);
    void showEditWaypointMenu(const QPoint& cursorPos);

    enum Mode {
        DEFAULT_MODE,
        MOVE_WAYPOINT_MODE
    };
    Mode mode;
    int selectedWpIndex;

    bool displayGrid;
    bool displayTrail;
    bool displayImagery;
    bool displayTarget;
    bool displayWaypoints;
    bool displayRGBD2D;
    bool displayRGBD3D;
    bool enableRGBDColor;

    bool followCamera;

    osg::ref_ptr<osg::Vec3dArray> trailVertices;
    QVarLengthArray<osg::Vec3d, 10000> trail;

    osg::ref_ptr<osg::Node> vehicleModel;
    osg::ref_ptr<osg::Geometry> hudBackgroundGeometry;
    osg::ref_ptr<osgText::Text> statusText;
    osg::ref_ptr<ImageWindowGeode> rgb2DGeode;
    osg::ref_ptr<ImageWindowGeode> depth2DGeode;
    osg::ref_ptr<osg::Image> rgbImage;
    osg::ref_ptr<osg::Image> depthImage;
    osg::ref_ptr<osg::Geode> gridNode;
    osg::ref_ptr<osg::Geode> trailNode;
    osg::ref_ptr<osg::Geometry> trailGeometry;
    osg::ref_ptr<osg::DrawArrays> trailDrawArrays;
    osg::ref_ptr<Imagery> mapNode;
    osg::ref_ptr<osg::Geode> targetNode;
    osg::ref_ptr<osg::PositionAttitudeTransform> targetPosition;
    osg::ref_ptr<osg::Group> waypointsNode;
    osg::ref_ptr<osg::Geode> rgbd3DNode;
#ifdef QGC_LIBFREENECT_ENABLED
    QScopedPointer<Freenect> freenect;
    QVector<Freenect::Vector6D> pointCloud;
#endif
    bool enableFreenect;
    QSharedPointer<QByteArray> rgb;
    QSharedPointer<QByteArray> coloredDepth;

    QVector< osg::ref_ptr<osg::Node> > vehicleModels;

    QPushButton* targetButton;

    double lastRobotX, lastRobotY, lastRobotZ;
};

#endif // PIXHAWK3DWIDGET_H
