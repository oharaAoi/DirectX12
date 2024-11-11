#include "GpuParticle.h"

GpuParticle::GpuParticle() {
}

GpuParticle::~GpuParticle() {
}

void GpuParticle::Init() {
	particleBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(Particle) * 1024);
}

void GpuParticle::Update() {
}

void GpuParticle::Draw() {
}
