#include <maya/MFnPlugin.h>
#include "gerstnerWaveDeformer.h"




MStatus initializePlugin(MObject obj) {
    MStatus status;
    MFnPlugin fnPlugin(obj, "Luke Boscott", "1.0", "Any");

    status = fnPlugin.registerNode("gerstnerWave",
        GerstnerWave::id,
        GerstnerWave::creator,
        GerstnerWave::initialise,
        MPxNode::kDeformerNode);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj) {
    MStatus   status;
    MFnPlugin fnPlugin(obj);

    status = fnPlugin.deregisterNode(GerstnerWave::id);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return MS::kSuccess;
}