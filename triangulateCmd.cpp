//-
// ==========================================================================
// Copyright pku&itu . All rights reserved.
// ==========================================================================
//+
// 
// File: trangulateCmd.cpp
//
// 
// Author: Lingbo Wang
//

#include "trangulateCmd.h"


MStatus trangulateCmd::doIt ( const MArgList &args )
{
	MStatus status = MS::kSuccess;
	MItDag dagIterator( MItDag::kBreadthFirst, MFn::kInvalid, &status);

	if ( MS::kSuccess != status) {
		fprintf(stderr,"Failure in DAG iterator setup.\n");
		return MS::kFailure;
	}


	for ( ; !dagIterator.isDone(); dagIterator.next() )
	{
		MDagPath dagPath;
		MObject  component = MObject::kNullObj;
		status = dagIterator.getPath(dagPath);

		if (!status) {
			fprintf(stderr,"Failure getting DAG path.\n");
			return MS::kFailure;
		}

		// skip over intermediate objects
		//
		MFnDagNode dagNode( dagPath, &status );
		if (dagNode.isIntermediateObject()) 
		{
			continue;
		}

		if ((  dagPath.hasFn(MFn::kNurbsSurface)) &&
			(  dagPath.hasFn(MFn::kTransform)))
		{
			status = MS::kSuccess;
			fprintf(stderr,"Warning: skipping Nurbs Surface.\n");
		}
		else if ((  dagPath.hasFn(MFn::kMesh)) &&
			(  dagPath.hasFn(MFn::kTransform)))
		{
			// We want only the shape, 
			// not the transform-extended-to-shape.
			continue;
		}
		else if (  dagPath.hasFn(MFn::kMesh))
		{

			MString meshName = dagPath.fullPathName();
			MString trangulateCmdStr = MString("polyTriangulate ")+meshName;
			MGlobal::displayInfo( meshName );
			MGlobal::executeCommand(trangulateCmdStr);
		//	cout << meshName.asChar();
		//	printf("%s", meshName.asChar());
		}
	}

	return MS::kSuccess;
}