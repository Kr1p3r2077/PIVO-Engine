#pragma once
#include "Material.h"
#include "Model.h"

class ModelRenderer {
	Model model;
	Material material;

	ModelRenderer(Model &mod, Material &mat) {
		model = mod;
		material = mat;
	}
};