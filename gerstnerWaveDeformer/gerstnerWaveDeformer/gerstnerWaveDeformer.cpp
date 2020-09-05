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
	nPoints = 0;
	initialised = false;
}

GerstnerWave::~GerstnerWave()
{

}

void* GerstnerWave::creator()
{
	return new GerstnerWave();
}

void GerstnerWave::init(MItGeometry& itGeo)
{
	initialised = true;
	for (; !itGeo.isDone(); itGeo.next())
	{
		points[nPoints++] = itGeo.position();
	}

}

MStatus  GerstnerWave::deform(MDataBlock& data, MItGeometry& itGeo,
	const MMatrix& localToWorldMatrix,
	unsigned int geomIndex)
{
	if (!initialised)
		init(itGeo);

	MStatus status;

	float waveLength = data.inputValue(aWaveLength).asFloat();
	float numWaves = data.inputValue(aNumWaves).asFloat();
	float steepness = data.inputValue(aSteepness).asFloat();
	float amplitude = data.inputValue(aAmplitude).asFloat();
	float decay = data.inputValue(aDecay).asFloat();
	float speed = data.inputValue(aSpeed).asFloat();
	float frame = data.inputValue(aFrame).asFloat();
	float* aNewDirection = data.inputValue(aDirection).asFloat3();

	MPoint point;

	MVector direction(aNewDirection);
	direction.y = 0;
	direction.normalize();

	int currentPoint = 0;
	for (; !itGeo.isDone(); itGeo.next())
	{
		point = points[currentPoint];

		double wl =  2 / waveLength;

		double dotXY = direction * point;

		double Q = steepness / ((wl * amplitude) * numWaves);
		double phase = (speed * wl) * frame;

		double waveX = (Q * amplitude) * direction.x * cos(wl * dotXY + phase);
		double waveZ = amplitude * sin((waveLength * dotXY) + phase);
		double waveY = (Q * amplitude) * direction.z * cos(wl * dotXY + phase);

		MPoint wave(waveX, waveZ, waveY);

		point += wave;

		itGeo.setPosition(point);
		currentPoint++;
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

	aNumWaves = nAttr.create("numWaves", "numWaves", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aNumWaves);
	attributeAffects(aNumWaves, outputGeom);

	aSteepness = nAttr.create("steepness", "steepness", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aSteepness);
	attributeAffects(aSteepness, outputGeom);

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