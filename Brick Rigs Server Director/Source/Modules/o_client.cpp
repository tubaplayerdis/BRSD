#include "../../Include/Modules/o_client.h"
#include "../../Include/Global/Global.h"

#include <SDK/BrickRigs_classes.hpp>
#include <SDK/Engine_classes.hpp>
#include <socketio/sio_client.h>

namespace
{
    std::unique_ptr<o_client> base;
}

o_client::o_client() : client_()
{
    events_ = std::vector<o_event>();

    #ifdef _DEBUG
    client_.connect("http://127.0.0.1:3000");
    #else
    client_.connect("https://aaronwilk.dev:3000");
    #endif

    client_.socket()->on_any([this](sio::event& e) -> void
    {
        for (auto& event : events_)
        {
            if (e.get_name() == event.get_name())
            {
                if (!SDK::UKismetSystemLibrary::IsServer(SDK::UWorld::GetWorld())) return;
                event.execute(e);
            }
        }
    });

    client_.socket()->on("identify", [&](sio::event& ev)
    {
        SDK::ABrickGameState* state = SDK::ABrickGameState::Get(SDK::UWorld::GetWorld());

        if (!state) return; //The message will timeout

        std::string host_identity = "None";
        std::string host_name = "None";
        std::string server_name = state->GetMatchSettings().ServerName.ToString();

        for (const auto player : state->PlayerArray)
        {
            if (reinterpret_cast<SDK::ABrickPlayerState*>(player)->GetAdminRole() == SDK::EAdminRole::Owner)
            {
                host_identity = SDK::UBrickStatics::UniqueNetIdToString(player->UniqueId).ToString();
                host_name = player->GetPlayerName().ToString();
            }
        }

        const sio::message::ptr packet = sio::object_message::create();
        packet->get_map()["HostID"] = sio::string_message::create(host_identity);
        packet->get_map()["HostName"] = sio::string_message::create(host_name);
        packet->get_map()["ServerName"] = sio::string_message::create(server_name);

        ev.put_ack_message(packet);
    });
}

o_client::~o_client()
{
    client_.sync_close();
}

o_client* o_client::get()
{
    if (!base) base = std::unique_ptr<o_client>(new o_client());
    return base.get();
}

void o_client::event_register(const o_event& e)
{
    events_.push_back(e);
}

void o_client::message_emit(std::string const& name, sio::message::list const& msglist, std::function<void (sio::message::list const&)> const& ack)
{
    client_.socket()->emit(name, msglist, ack);
}

o_event::o_event(const std::string& name, const std::function<void(sio::event&)>& function) : name_(name), function_(function)
{
    o_client::get()->event_register(*this);
}