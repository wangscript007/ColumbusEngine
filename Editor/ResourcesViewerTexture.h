#pragma once

#include <Scene/Scene.h>
#include <Graphics/Texture.h>

namespace Columbus
{

	class ResourcesViewerTexture
	{
	private:
		static bool Opened;
		static Texture** Destination;
		static Texture* Tmp;
	public:
		static void Open(Texture** Dst)
		{
			Opened = true;
			Destination = Dst;
		}

		static void Close()
		{
			Opened = false;
			if (Destination != nullptr)
			{
				*Destination = Tmp;
			}
		}

		static void Draw(Scene* Scn);
	};

}


