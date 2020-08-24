#include "gerstnerWaveDeformer.h"

MTypeId GerstnerWave::id(0x00000000);
MObject GerstnerWave::aMesh;
MObject GerstnerWave::aFrame;
MObject GerstnerWave::aNumWaves;
MObject GerstnerWave::aSteepness;

MObject GerstnerWave::aDirection;
MObject GerstnerWave::aAmplitude;
MObject GerstnerWave::aWaveLength;
MObject GerstnerWave::aDecay;
MObject GerstnerWave::aSpeed;


GerstnerWave::GerstnerWave() 
{

}

GerstnerWave::~GerstnerWave()
{

}

void* GerstnerWave::creator()
{
	return new GerstnerWave();
}


MStatus  GerstnerWave::deform( MDataBlock& data, MItGeometry& itGeo,
								const MMatrix& localToWorldMatrix,
								unsigned int geomIndex)
{
	MStatus status;

	MDataHandle hMesh = data.inputValue(aMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MObject oMesh = hMesh.asMesh();
	if (oMesh.isNull())
	{
		return MS::kSuccess;
	}

	MFnMesh fnMesh(oMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MPointArray thisMesh;
	fnMesh.getPoints(thisMesh);

	float waveLength = data.inputValue(aWaveLength).asFloat();
	float numWaves = data.inputValue(aNumWaves).asFloat();
	float steepness = data.inputValue(aSteepness).asFloat();
	float amplitude = data.inputValue(aAmplitude).asFloat();
	float decay = data.inputValue(aDecay).asFloat();
	float speed = data.inputValue(aSpeed).asFloat();
	float frame = data.inputValue(aFrame).asFloat();
	float* aNewDirection = data.inputValue(aDirection).asFloat3();

	MFloatVector direction;
	direction.get(aNewDirection);

	double wl = (M_PI * 2) / waveLength;
	double nWaves = (M_PI * 2) * numWaves;

	float dotDirection = direction * direction;

	float Q = steepness / (waveLength * amplitude) * numWaves;
	float phase = (speed * waveLength) * frame;
	
	float repeater = (dotDirection * waveLength) + phase;
	float decayer = exp(decay * dotDirection);

	double cosRep = cos(repeater);
	double sinRep = sin(repeater);
	double dirX = direction(0);
	double dirY = direction(1);
	double dirXCosRep = dirX * cosRep;
	double dirYCosRep = dirY * cosRep;
	float middleMul = amplitude * decay * sinRep;



	MPoint wave (Q * (amplitude * dirXCosRep),
		middleMul,
		Q * (amplitude * dirYCosRep));

	MPoint point;
	for (; !itGeo.isDone(); itGeo.next())
	{
		point = itGeo.position();

		point += (thisMesh[itGeo.index()] - point) + wave;

		itGeo.setPosition(point);
	}


	return MS::kSuccess;
}

MStatus GerstnerWave::initialise()
{
	MStatus status;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	

	aMesh = tAttr.create("waveMesh", "waveMesh", MFnData::kMesh);
	addAttribute(aMesh);
	attributeAffects(aMesh, outputGeom);

	aFrame = nAttr.create("frameValue", "frameValue", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aFrame);
	attributeAffects(aFrame, outputGeom);

	aDirection = nAttr.create("direction", "direction", MFnNumericData::k3Float);
	nAttr.setKeyable(true);
	addAttribute(aDirection);
	attributeAffects(aDirection, outputGeom);

	aAmplitude = nAttr.create("amplitude", "amplitude", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aAmplitude);
	attributeAffects(aAmplitude, outputGeom);

	aWaveLength = nAttr.create("waveLength", "waveLength", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aWaveLength);
	attributeAffects(aWaveLength, outputGeom);

	aDecay = nAttr.create("decay", "decay", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aDecay);
	attributeAffects(aDecay, outputGeom);

	aSpeed = nAttr.create("speed", "speed", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aSpeed);
	attributeAffects(aSpeed, outputGeom);


	return MS::kSuccess;
}