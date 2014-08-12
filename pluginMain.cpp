//
// Copyright (C) 2013 Bubbler Wong 
// 
//#include "MeltNode.h"
#include "cameraControlCmd.h"
#include "cmdDispatch.h"
#include "objExport.h"
#include "trangulateCmd.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{ 
	MStatus stat;
	MString errStr;
	MFnPlugin plugin( obj, "David Gould", "1.0", "Any");

//===register Commands===========
	stat = plugin.registerCommand( "cameraControl", cameraControlCmd::creator );
	if ( !stat )
	{
		errStr = "registerCommand failed";
		goto error;
	}
	stat = plugin.registerCommand( "augReality", cmdDispatch::creator );
	if ( !stat )
	{
		errStr = "registerCommand failed";
		goto error;
	}
	stat = plugin.registerCommand( "triangulateCmd", trangulateCmd::creator );
	if ( !stat )
	{
		errStr = "registerCommand failed";
		goto error;
	}






//==========register nodes============
	//stat = plugin.registerNode( "melt", MeltNode::id, MeltNode::creator, MeltNode::initialize );
	//if ( !stat ) 
	//{
	//	errStr = "registerNode failed";
	//	goto error;
	//}

//====================register file translator=====
	stat = plugin.registerFileTranslator( "OBJexport", "none", ObjTranslator::creator, (char *)objOptionScript, (char *)objDefaultOptions ); 
	if(!stat)
	{
		errStr = "export obj failed";
		goto error;
	}


	return stat;

error:
	stat.perror( errStr );
	return stat;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin plugin( obj );

	//========deregister commands=============
	stat = plugin.deregisterCommand( "melt" );
	if ( !stat )
	{
		errStr = "deregisterCommand failed";
		goto error;
	}

	stat = plugin.deregisterCommand( "augReality" );
	if ( !stat )
	{
		errStr = "deregisterCommand failed";
		goto error;
	}

	stat = plugin.deregisterCommand( "trangulateCmd" );
	if ( !stat )
	{
		errStr = "deregisterCommand failed";
		goto error;
	}


	//============deregister nodes===================
/*
	stat = plugin.deregisterNode( MeltNode::id );
	if( !stat ) 
	{
		errStr = "deregisterNode failed";
		goto error;
	}
*/
	//=======deregister file translator=================
	stat = plugin.deregisterFileTranslator("OBJexport");
	if(!stat)
	{
		errStr = "deregister export obj failed";
		goto error;
	}

	return stat;

error:
	stat.perror( errStr );
	return stat;
}
