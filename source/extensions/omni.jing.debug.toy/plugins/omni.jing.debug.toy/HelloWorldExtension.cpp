// Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto.  Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#define CARB_EXPORTS

#include <carb/PluginUtils.h>

#include <omni/ext/IExt.h>
#include <omni/kit/IApp.h>

const struct carb::PluginImplDesc pluginImplDesc = { "omni.jing.debug.toy",
                                                     "An example C++ extension.", "NVIDIA",
                                                     carb::PluginHotReload::eEnabled, "dev" };

CARB_PLUGIN_IMPL_DEPS(omni::kit::IApp)

namespace omni
{

// When this extension is enabled, any class that derives from omni.ext.IExt
// will be instantiated and 'onStartup(extId)' called. When the extension is
// later disabled, a matching 'onShutdown()' call will be made on the object.
char* ptr;

class ExampleCppHelloWorldExtension : public omni::ext::IExt
{
public:
    void onStartup(const char* extId) override
    {
        printf("ExampleCppHelloWorldExtension starting up (ext_id: %s).\n", extId);

        // try to crash with "free(): corrupted unsorted chunks"
        ptr = (char*)malloc(66666);
        free(ptr);

        printf("Did Kit crash?\n");

        // int *arr = NULL;
        // *arr = 0;

        // Get the app interface from the Carbonite Framework.
        if (omni::kit::IApp* app = carb::getFramework()->acquireInterface<omni::kit::IApp>())
        {
            // Subscribe to update events.
            m_updateEventsSubscription =
                carb::events::createSubscriptionToPop(app->getUpdateEventStream(), [this](carb::events::IEvent*) {
                onUpdate();
            });
        }
    }

    void onShutdown() override
    {
        printf("ExampleCppHelloWorldExtension shutting down.\n");

        // Unsubscribe from update events.
        m_updateEventsSubscription = nullptr;
    }

    void onUpdate()
    {
        if (m_updateCounter % 100 == 0)
        {
            free(ptr);

            printf("Hello from the omni.jing.debug.tool extension! %d updates counted.\n", m_updateCounter);
        }
        m_updateCounter++;
    }

private:
    carb::events::ISubscriptionPtr m_updateEventsSubscription;
    int m_updateCounter = 0;
};

}

CARB_PLUGIN_IMPL(pluginImplDesc, omni::ExampleCppHelloWorldExtension)

void fillInterface(omni::ExampleCppHelloWorldExtension& iface)
{
}
