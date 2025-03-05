#include "studio.h"

#include "core/interfaces.h"
#include "interfaces/imdlcache.h"

const studiohdr_t* virtualgroup_t::GetStudioHdr() const
{
	return I::MDLCache->GetStudioHdr(static_cast<MDLHandle_t>(reinterpret_cast<intptr_t>(this->pCache)));
}

virtualmodel_t* studiohdr_t::GetVirtualModel() const
{
	if (nIncludeModelCount == 0)
		return nullptr;

	return I::MDLCache->GetVirtualModelFast(this, static_cast<MDLHandle_t>(reinterpret_cast<intptr_t>(GetVirtualModelHandle())));
}
