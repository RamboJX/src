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

#ifndef TRANGULATECMD_H
#define TRANGULATECMD_H

#include <maya/MPxCommand.h>
#include <maya/MDagPath.h>
#include <maya/MStatus.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MGlobal.h>
#include <iostream>
#include <maya/MString.h>

class trangulateCmd : public MPxCommand
{
public:
		MStatus doIt( const MArgList& args );
		static void* creator(){return new trangulateCmd;}
};

#endif
