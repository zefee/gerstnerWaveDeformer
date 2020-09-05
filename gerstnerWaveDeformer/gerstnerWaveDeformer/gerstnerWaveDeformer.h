#pragma once
#include <maya/MPxDeformerNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MItGeometry.h>
#include <maya/MFloatVector.h>
#include <math.h>


class GerstnerWave : public MPxDeformerNode
{
public:
	GerstnerWave();
	~GerstnerWave();
	static void* creator();

	virtual MStatus deform( MDataBlock& data,
							MItGeometry& itGeo,
							const MMatrix& localToWorldMatrix,
							unsigned int geomIndex);

	static MStatus initialise();

	bool initialised;
	void init(MItGeometry& itGeo);
	MPoint points[50000];
	int nPoints;

	static MTypeId id;

	static MObject aMesh;
	static MObject aFrame;
	static MObject aNumWaves;
	static MObject aSteepness;

	static MObject aDirection;
	static MObject aAmplitude;
	static MObject aWaveLength;
	static MObject aDecay;
	static MObject aSpeed;
};
