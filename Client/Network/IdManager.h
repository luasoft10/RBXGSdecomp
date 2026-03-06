#pragma once
#include "reflection/reflection.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"
#include "util/Guid.h"
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <map>

namespace RBX
{
	extern const char* sIdManager;

	class IdManager : public DescribedNonCreatable<IdManager, Instance, &sIdManager>, public Service
	{
	private:
		std::map<Guid::Data, Instance*> items;
		boost::signals::scoped_connection removeInstanceConnection;
	public:
		Instance* getInstance(Guid::Data id);
		void addInstance(Instance* instance, Guid::Data explicitId);
		void addInstance(Instance* instance);
	protected:
		virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	private:
		void removeInstance(boost::shared_ptr<Instance> instance);
	};
}
