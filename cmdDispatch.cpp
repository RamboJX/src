#include "cmdDispatch.h"
#include <maya/MFnAnimCurve.h>
#include <maya/M3dView.h>


#define MAX_CMD_LENGTH 16
#define MAX_PARAM_LENGTH 16

using namespace std;

Server cmdDispatch::m_tcpServ;
cmdUtil cmdDispatch::m_cmdUtil;
MDagPath cmdDispatch::perspectiveCamDagPath;
bool cmdDispatch::IsCmdRunning = false;
cameraAnimCurveFn cmdDispatch::activeCameraACFn;
MDagPath cmdDispatch::testTrans;


MStatus cmdDispatch::doIt ( const MArgList &args )
{ 	
	
	MStatus status = MS::kSuccess;

	// if the command is already running. don't open the thread again.
	if(IsCmdRunning){
		MGlobal::displayInfo( MString( "cmd is already running!" ) );	
		return MStatus::kSuccess;
	}
	else{
		IsCmdRunning = true;
	}

	// get optional flags
	MArgDatabase	argData( syntax(), args );
	parseSyntax( argData );
/*
	//////////////////////////////////////////////////////////////////////////
	//render part
	//
	//////////////////////////////////////////////////////////////////////////
	if(!MRenderView::doesRenderEditorExist())
	{
		return MS::kFailure;
	}

	M3dView curView = M3dView::active3dView();
	curView.getCamera( perspectiveCamDagPath );

	cout<<"rendering camera"<<perspectiveCamDagPath.fullPathName().asChar()<<endl;

	if(MRenderView::setCurrentCamera(perspectiveCamDagPath) != MS::kSuccess)
	{
		setResult("error occurred in setCurrentCamera");
		return MS::kFailure;
	}

	//render a 640*480 image.
	unsigned int image_width = 640, image_height = 480;
	if(MRenderView::startRender(image_width, image_height, doNotClearBackgroud)!= MS::kSuccess)
	{
		return MS::kFailure;
	}

	//draw render view









	//initial to get the activity camera dag-path
	// which use to view but not to set key frame.
	//TODO: make the new camera's view change.
	//MStatus stat = M3dView::active3dView().getCamera(perspectiveCam);

	//this attributes just read from maya, we should 
	//TODO: get this attributes value from virtrue camera

	

	


	//MString transformNodeName;
	//if(!recFindTransformDAGNodes(  perspectiveCam.fullPathName(),  transformNodeName))
	//	cout<<" i can't find the transform node!!!"<<endl;
	//cout<<"Rendering camera"<<perspectiveCam.fullPathName().asChar()<<endl;
	//cout<<"transform node name is"<<transformNodeName.asChar()<<endl;
	
	*/

//////////////////////////////////////////////////////////////////////////
//1. new a camera, and set the animation fn bind to this camera
	MString camera_name = "test";
	MFnCamera cameraFn;
	MObject camera = cameraFn.create(&status);
	if (status == MS::kFailure) printf("Error creating the camera.\n");
	MFnDagNode fnSet( camera, &status);
	//if ( MS::kSuccess != status ) printf("Failure to create function set\n");
	MDagPath dp;
	if (cameraFn.getPath(dp) != MS::kSuccess) printf("Failure creating dag path\n");
	if (camera_name.length() > 0) {
		fnSet.setName(camera_name, &status);
		if (status == MS::kFailure) printf("Error setting the camera name.\n");
	}


//2. bind
	// init translateX animation curve fn
	MString attrName( "translateX");
	const MObject attrtx = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute translateX\n");	
	activeCameraACFn.acFnSetTx.create ( dp.transform(), attrtx, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (translateX)\n");

	//init translateY animation curve fn
	attrName.set( "translateY");
	const MObject attrty = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute translateY\n");
	activeCameraACFn.acFnSetTy.create ( dp.transform(), attrty, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (translateX)\n");

	//init translateZ animation curve fn
	attrName.set( "translateZ");
	const MObject attrtz = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute translateZ\n");
	activeCameraACFn.acFnSetTz.create ( dp.transform(), attrtz, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (translateY)\n");

	//init rotateX animation curve fn
	attrName.set( "rotateX");
	const MObject attrrx = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute rotateX\n");
	activeCameraACFn.acFnSetRx.create ( dp.transform(), attrrx, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (translateZ)\n");

	//init rotateY animation curve fn
	attrName.set( "rotateY");
	const MObject attrry = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute rotateY\n");
	activeCameraACFn.acFnSetRy.create ( dp.transform(), attrry, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (rotateY)\n");

	//init rotateZ animation curve fn
	attrName.set( "rotateZ");
	const MObject attrrz = fnSet.attribute (attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute rotateZ\n");
	activeCameraACFn.acFnSetRz.create ( dp.transform(), attrrz, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set (rotateZ)\n");

	//init horizontal film aperture fn
	attrName.set( "horizontalFilmAperture" );
	const MObject attrhfa = fnSet.attribute( attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute hfa\n");
	activeCameraACFn.acFnSetHfa.create ( dp.node(), attrhfa, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set hfa\n");

	//init vertical film aperture fn
	attrName.set( "verticalFilmAperture" );
	const MObject attrvfa = fnSet.attribute( attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute vfa\n");
	activeCameraACFn.acFnSetVfa.create ( dp.node(), attrvfa, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set vfa\n");

	//init focal length fn
	attrName.set( "focalLength" );
	const MObject attrfl = fnSet.attribute( attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute fl\n");
	activeCameraACFn.acFnSetFl.create ( dp.node(), attrfl, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set focal length\n");

	//init fstop fn
	attrName.set( "fStop" );
	const MObject attrfs = fnSet.attribute( attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute fstop\n");
	activeCameraACFn.acFnSetFs.create ( dp.node(), attrfs, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set fstop\n");

	//init focus distance fn
	attrName.set( "focusDistance" );
	const MObject attrfd = fnSet.attribute( attrName, &status);
	if ( MS::kSuccess != status ) printf( "Failure to find attribute fd\n");
	activeCameraACFn.acFnSetFd.create ( dp.node(), attrfd, NULL, &status);
	if ( MS::kSuccess != status ) printf("Failure creating MFnAnimCurve function set fd\n");

//////////////////////////////////////////////////////////////////////////

	this->tcpRoutine();
	return MStatus::kSuccess;
}

MSyntax cmdDispatch::newSyntax()
{
	MStatus status;
	MSyntax syntax;
	syntax.addFlag( kDoNotClearBackground, kDoNotClearBackgroundLong );
	CHECK_MSTATUS_AND_RETURN(status, syntax);
	return syntax;
}




MStatus cmdDispatch::parseSyntax(MArgDatabase &argData)
{
	doNotClearBackgroud = argData.isFlagSet(kDoNotClearBackground);
	return MS::kSuccess;
}

MStatus cmdDispatch::tcpRoutine()
{
	//build a socket
	m_tcpServ.CSocket();
	//bind the socket
	m_tcpServ.CBind();
	m_tcpServ.CListen();
    
    
    
    pthread_t thread;
//	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)answerThread, NULL, 0, NULL);
	
    pthread_create( &thread, NULL, answerThread, NULL);
    
    if(thread == NULL)
	{
		MGlobal::displayError( MString( "create thread error" ) );	
		m_tcpServ.CClose();
		return MStatus::kFailure;
	}
	return MStatus::kSuccess;
}


//receive and execute commands
void *cmdDispatch::answerThread(void *arg){

	while(1)
	{
		while( m_tcpServ.CAccept() != 1 )	// when accept a new connect
			;
		while( strcmp(m_tcpServ.bufRecv , "exit")!=0 )
		{
			if( m_tcpServ.CRecv() == 1)
			{
			//===	parseAndDispatch();==========================

				MString cmdLine(m_tcpServ.bufRecv);
				MStringArray cmdWords;

				cmdLine.split(';',cmdWords);

				cout<<"this is the received command line:";
				cout<<cmdLine<<endl;
				//=============parse====================
				if( cmdWords[0] == "camera")
				{
			// step 1. get camera attribute
					cameraParam cameraPara;
					double timeFrame  = cmdWords[TIME_FRAME].asDouble();
					cameraPara.translateX = cmdWords[TRANS_X].asFloat();
					cameraPara.translateY = cmdWords[TRANS_Y].asFloat();
					cameraPara.translateZ = cmdWords[TRANS_Z].asFloat();

					//from quaternion to Euler
					double quat_x = cmdWords[QUATERNION_X].asFloat();
					double quat_y = cmdWords[QUATERNION_Y].asFloat();
					double quat_z = cmdWords[QUATERNION_Z].asFloat();
					double quat_w = cmdWords[QUATERNION_W].asFloat();

					MQuaternion quater(quat_x, quat_y, quat_z, quat_w);
					MEulerRotation euler = quater.asEulerRotation();

					cameraPara.rotateX = euler.x;
					cameraPara.rotateY = euler.y;
					cameraPara.rotateZ = euler.z;


					//MFnTransform fnPerspectiveCamera(persTransDagpath);


					
			//step 2. set the perspective camera for synchro display 
					MFnCamera fnPerspectiveCamera(perspectiveCamDagPath);
					//rotate the camera
					MVector oldViewDirection;
					MVector newViewDirection;
					MVector oldUpDirection;
					MVector newUpDirection;
					oldViewDirection = fnPerspectiveCamera.viewDirection();
					newViewDirection = oldViewDirection.rotateBy(quater);
					oldUpDirection = fnPerspectiveCamera.upDirection();
					newUpDirection = oldUpDirection.rotateBy(quater);

					//fnPerspectiveCamera.set();
					fnPerspectiveCamera.setEyePoint(MPoint(cameraPara.translateX, cameraPara.translateY,cameraPara.translateZ), MSpace::kWorld);

					M3dView curView = M3dView::active3dView();
					curView.refresh(true);


				//	MPoint eyePt (cameraPara.translateX, cameraPara.translateY, cameraPara.translateZ);
				//	fnPerspectiveCamera.setEyePoint(eyePt);


			//setp 3. key the camera 
					
					activeCameraACFn.acFnSetTx.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.translateX);
					activeCameraACFn.acFnSetTy.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.translateY);
					activeCameraACFn.acFnSetTz.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.translateZ);

					activeCameraACFn.acFnSetRx.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.rotateX);
					activeCameraACFn.acFnSetRy.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.rotateY);
					activeCameraACFn.acFnSetRz.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.rotateZ);
					
					/*
						activeCameraACFn.acFnSetHfa.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.horizonlFilmAperture);
						activeCameraACFn.acFnSetVfa.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.verticalFilmAperture);
						activeCameraACFn.acFnSetFl.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.focalLength);
						activeCameraACFn.acFnSetFs.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.fstop);
						activeCameraACFn.acFnSetFd.addKeyframe(MTime(timeFrame,MTime::uiUnit()),cameraPara.focusDistance);
						*/


					
				}
				else if(cmdWords[0] == "kcamera")	// this command is just set the camera's postion 
				{
					//set the camera status for the cmd received
					//m_cmdUtil.cameraSet(wsEyeLocation, wsViewDirection, wsUpDirection, horizFieldOfView, aspectRatio);
					//MGlobal::viewFrame(mTimeFrame);

				}
				else
					MGlobal::displayInfo( cmdLine );


			}
			else if( m_tcpServ.CRecv() == -1 ) // receive over, client close
			{
				break;
			}
			//else receive the next line; ok?
		}
		//m_tcpServ.CSend();
	}
}



bool cmdDispatch::recFindTransformDAGNodes( MString& nodeName, MString& transformNodeName)
{
	// To handle Maya groups we traverse the hierarchy starting at
	// each objectNames[i] going towards the root collecting transform
	// nodes as we go.
	MStringArray result;
	MString cmdStr = "listRelatives -ap " + nodeName;

	cout << cmdStr.asChar()<<endl;

	MGlobal::executeCommand( cmdStr, result );

	if( result.length() == 0 )
		// nodeName must be at the root of the DAG.  Stop recursing
		return false;

	for( unsigned int j=0; j<result.length(); j++ ) {
		// check if the node result[i] is of type transform
		MStringArray result2;
		MGlobal::executeCommand( "nodeType " + result[j], result2 );

		if( result2.length() == 1 && result2[0] == "transform" ) {
			// check if result[j] is already in result[j]
			transformNodeName = result[j];
			return true;
		}
	}
	return false;
}