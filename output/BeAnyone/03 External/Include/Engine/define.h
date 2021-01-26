#pragma once

// ╫л╠шео
#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

#define SAFE_RELEASE(p) if(nullptr != p) p->Release()
#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

#define DEVICE CDevice::GetInst()->GetDevice()




enum class RES_TYPE
{
	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	SHADER,
	END,
};