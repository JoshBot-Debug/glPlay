# TODO


Check the max size of the VBO

int maxVBOSize;
glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &maxVBOSize);

// Create a createBatch() method
// The Batch class will contain a class called InstanceManager
// InstanceManager will be passed into model so you can create a new instance of a model from a model
// InstanceManager will contain a DynamicBuffer for vertex buffers
// DynamicBuffer will resize a buffer if there is not enough space
// If DynamicBuffer hits the max buffer size, Batch will create a new DynamicBuffer to hold the spillover
// Batch will keep track of what model, where the instances are stored, etc
// DynamicBuffer, may want to implement double buffering / triple buffering
Batch *batch = engine.createBatch();

// batch can add a model to the batch
Model *model = batch->addModel("/path/to/model")

// From a model you can create an instance.
// Save the pointer to the instance to update it's 
// transforms.
// Also add a way to reserver memory for x amount of instances
model.reserve(100);
Instance *instance = model.createInstance();
instance.position.x = 0.0f;
instance.position.y = 0.0f;

#TODO
Read raylib

