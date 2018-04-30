// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AllowWindowsPlatformTypes.h"
#include <vector>
#include "HideWindowsPlatformTypes.h"
#include "UnrealMath.h"

/**
 * 
 */
class VOXELGEOMETRY_API PerlinNoise {
	// The permutation vector
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);
	//FVector Version
	double noise(FVector V) { return noise(V.X, V.Y, V.Z); }
	//Octave Perlin Noise
	double Octave(FVector Pos, int octaves, float persistence);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};