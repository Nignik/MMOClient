#include "GameClient.h"

int main()
{
	GameClient client;
	client.OnUserCreate();

	while (true)
	{
		client.Update();
	}
}