#ifndef CMDDISPATCH_H
#define CMDDISPATCH_H

#include <maya/MPxCommand.h>
#include <maya/MDagPath.h>
#include <maya/MFnAnimCurve.h>

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MTime.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnCamera.h>
#include <maya/MFnAnimCurve.h>
#include <maya/M3dView.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnTransform.h>
#include <maya/MFnMesh.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderView.h>
#include <maya/MSyntax.h>




#include "TCPServer.h"
#include "cmdUtil.h"

#define TIME_FRAME	1
#define LOC_X		3
#define LOC_Y		4
#define LOC_Z		5
#define VIEW_DIR_X	6
#define VIEW_DIR_Y	7
#define VIEW_DIR_Z	8
#define UP_DIR_X	9
#define UP_DIR_Y	10
#define UP_DIR_Z	11
#define HOV			12
#define ASPECTRATIO	13


//============the camera parameters=========
#define TRANS_X		2
#define TRANS_Y		3
#define TRANS_Z		4
#define QUATERNION_X	5
#define QUATERNION_Y	6
#define QUATERNION_Z	7
#define QUATERNION_W	8




static const char * kDoNotClearBackground		= "-b";
static const char * kDoNotClearBackgroundLong	= "-background";



struct cameraParam{
	int frame;
	double translateX, translateY, translateZ, rotateX, rotateY, rotateZ, scaleX, scaleY, scaleZ;
	double horizonlFilmAperture, verticalFilmAperture;
	int focalLength;
	double fstop, focusDistance;
};


struct cameraAnimCurveFn{
	MFnAnimCurve acFnSetTx;
	MFnAnimCurve acFnSetTy;
	MFnAnimCurve acFnSetTz;
	MFnAnimCurve acFnSetRx;
	MFnAnimCurve acFnSetRy;
	MFnAnimCurve acFnSetRz;
	MFnAnimCurve acFnSetHfa;
	MFnAnimCurve acFnSetVfa;
	MFnAnimCurve acFnSetFl;
	MFnAnimCurve acFnSetFs;
	MFnAnimCurve acFnSetFd;
};


class cmdDispatch : public MPxCommand 
{
public:
	virtual MStatus	doIt ( const MArgList& );
	static void *creator() { return new cmdDispatch; }
	static MSyntax newSyntax();

	MStatus parseSyntax(MArgDatabase &argData);

	
	//the routine of initialize tcp server 
	MStatus tcpRoutine();
    static void *answerThread(void *arg);

	static MStatus bindCamFn(MFnDagNode fnSet, MDagPath dp);

	bool recFindTransformDAGNodes( MString& nodeName, MString& transformNodeIndicesArray);

//	static bool parseAndDispatch();
//	static bool setCamera(MStringArray cmdWords, MDagPath camera);

	

private:
	static Server m_tcpServ;
	static bool IsCmdRunning;


	static cameraAnimCurveFn activeCameraACFn;
	static cmdUtil m_cmdUtil;
	static MDagPath perspectiveCamDagPath;

	bool doNotClearBackgroud;

	static MDagPath testTrans;


};


#endif