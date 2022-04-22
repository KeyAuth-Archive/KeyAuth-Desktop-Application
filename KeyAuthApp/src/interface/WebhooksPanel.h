#pragma once

#define array_size(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))  

namespace Interface::Webhook
{
	struct webhook_struct { char url[500], useragent[30]; bool authed; }; webhook_struct webhook;

	inline void __stdcall RenderWebhooks(void)
	{
		ImGui::Text("create a webhook");

		ImGui::InputText("webhook baseurl", webhook.url, array_size(webhook.url));
		ImGui::InputText("webhook useragent (default: KeyAuth)", webhook.useragent, array_size(webhook.useragent));
		
		ImGui::Checkbox("authed", &webhook.authed);

		if (ImGui::Button("create webhook"))
			Webhooks::CreateWebhook(webhook.url, webhook.useragent, webhook.authed);
	}
}