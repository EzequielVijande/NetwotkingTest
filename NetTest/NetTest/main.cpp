#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include "NetworkingModel.h"
#include "Server.h"
#include "Client.h"
#define MAX_IP_LENGTH 45
#define TEST1
//#define TEST2

int main(void)
{
#ifdef TEST1
	bool server= false;
	char ip[MAX_IP_LENGTH + 1]; //ip del otro jugador
	std::ifstream ip_file("./ip.txt");
	ip_file.getline(ip, MAX_IP_LENGTH); //Consigo la ip del otro jugador
	if (server)
	{
		Server S(PORT_S);
		S.setTurno(ESCUCHO);
		S.listening();
		getchar();

	}
	else
	{
		Client C(ip);
		C.startConnection(ip);
		C.sendSeq("holo");
		getchar();
	}


	return 0;

#endif //TEST1






#ifdef TEST2
	std::string sending;
	std::vector<char> pckg;
	bool sent = false;
	char ip[MAX_IP_LENGTH + 1]; //ip del otro jugador
	char user_name[256];
	NetworkingModel* NWM = new NetworkingModel();
	std::ifstream ip_file("./ip.txt");
	std::ifstream name_file("./temporal.txt");
	ip_file.getline(ip, MAX_IP_LENGTH); //Consigo la ip del otro jugador
	name_file.getline(user_name, 255); //Consigo el nombre de mi usuario.
	std::string user_nameS(user_name);

	NWM->setMe(user_nameS);
	srand(time(NULL));
	int waiting_time = 2000 + (rand() % 3000); //genera un tiempo de espera aleatorio entre 2000 y 5000 milisegundos.
	if ((NWM->connectAsClient(waiting_time, ip)))
	{
		NWM->setServer(CLIENT);
		pckg = NWM->readPackage();
		if (pckg[0] == NAME_HEADER)
		{
			std::string my_name = (NWM->getMe());
			unsigned char count = my_name.size();
			sending.append(1, NAME_IS_HEADER);
			sending.append(1, count);
			sending += my_name;
			do
			{
				sent = NWM->sendPackage((char*)sending.c_str(), sending.size());
			} while (!sent);
			pckg.clear();
			pckg = NWM->readPackage();
		}
	}
	else
	{
		if (NWM->connectAsServer()) //si tuvo exito manda el paquete de name
		{
			NWM->setServer(SERVER);
			sending.push_back(NAME_HEADER);
			do
			{
				sent = NWM->sendPackage((char*)sending.c_str(), 1);
			} while (!sent);
			pckg = NWM->readPackage();
			std::cout << std::endl << "Name of client is:" << std::endl;
			for (unsigned int i = 0; i < pckg.size(); i++)
			{
				std::cout << pckg[i];
			}
			getchar();
		}
	}
	delete NWM;
	
	return 0;

#endif //TEST2
}