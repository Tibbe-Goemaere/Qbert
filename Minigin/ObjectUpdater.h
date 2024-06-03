#pragma once

namespace dae
{
	class ObjectUpdater
	{
	public:
		virtual ~ObjectUpdater() = default;

		virtual void Update() = 0;
	};
}
