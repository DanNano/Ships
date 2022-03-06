//Main.cpp file for Lab 5 - Binary I/O
#include <iostream>
#include <fstream>
#include <climits> //Zybooks is requiring this in this file for it be compilied. Visual Studio and g++ didn't have issues with it. 
#include <vector> //Similar to Lab 4, I'll be using the vector format
using namespace::std;

//----------------Structs used in file------------------//
struct Ship 
{
	struct Weapons
	{
		string _WName; //String for weapon name
		int _Rating; //Int for power rating
		float _Power; //Float for power consumption 

		Weapons(string _Name, int _Rating, float _Power); //Function to set the "This" of the constructor, easier to hold generic data. 
	};

	float _Warp; //Float of the warp drive
	int _Shield; //int for shield
	int _FPower; //Power variable
	short _SLength; //Length of ship variable
	string _ShipName; //Ship name variable
	string _Class; //Ship class variable

	vector<Weapons> WMD;

	Ship(string _ShipName, string _Class, short _SLength, int _Shield, float _Warp, vector<Weapons>& WMD, int _ShipPower); //Function to set the "This" of the constructor

};

//---------------Function Prototypes----------------//
void Load(string fileName, vector<Ship>& Ships); //Function to load the the file into the program

void Print(vector<Ship>& Ships); //Function to print the information from the files

void Strong(vector<Ship>& Ships); //Function to find the strongest ship in a file

void StrongWMD(vector<Ship>& Ships); //Function to find the strongest weapons in a file

void Weak(vector<Ship>& Ships); //Function to find the weakest ships in a file

void UnArmed(vector<Ship>& Ships); //Function to find any unarmed ships in a file. 

//--------------------Main------------------//
int main()
{
	cout << "Which file(s) to open?\n";
	cout << "1. friendlyships.shp" << endl;
	cout << "2. enemyships.shp" << endl;
	cout << "3. Both files" << endl;
	int option;
	cin >> option;

	/* Load files here */
	vector<Ship> Ships;

	switch (option)
	{
	case 1:
		Load("friendlyships.shp", Ships);
		break;

	case 2:
		Load("enemyships.shp", Ships);
		break;

	case 3:
		Load("friendlyships.shp", Ships);
		Load("enemyships.shp", Ships);
		break;
	}

	cout << "1. Print all ships" << endl;
	cout << "2. Starship with the strongest weapon" << endl;
	cout << "3. Strongest starship overall" << endl;
	cout << "4. Weakest ship (ignoring unarmed)" << endl;
	cout << "5. Unarmed ships" << endl;

	cin >> option;

	/* Work your magic here */
	//Load("Magic.shp", Ships);

	switch (option)
	{
	case 1:
		Print(Ships);
		break;

	case 2:
		StrongWMD(Ships);
		break;

	case 3:
		Strong(Ships);
		break;

	case 4:
		Weak(Ships);
		break;

	case 5:
		UnArmed(Ships);
		break;
	}

	return 0;
}

//----------------Used Functions - Structs ---------------//

Ship::Ship(string _ShipName, string _Class, short _SLength, int _Shield, float _Warp, vector<Weapons>& WMD, int _FPower)
{
	this->_ShipName = _ShipName;
	this->_Class = _Class;
	this->_SLength = _SLength;
	this->_Shield = _Shield;
	this->_Warp = _Warp;
	this->WMD = WMD;
	this->_FPower = _FPower;
}

Ship::Weapons::Weapons(string _WName, int _Rating, float _Power)
{
	this->_WName = _WName;
	this->_Rating = _Rating;
	this->_Power = _Power;
}

//---------------Used Functions - Extras-----------------//

void Load(string fileName, vector<Ship>& Ships) //Load the file
{
	unsigned int _ShipCount;
	ifstream inFile(fileName, ios_base::binary);

	if (!inFile.is_open())
	{
		throw invalid_argument(fileName + "could not be opened. Please try again!");
		cout << endl;
	}

	inFile.read((char*)&_ShipCount, sizeof(_ShipCount));

	for (unsigned int i = 0; i < _ShipCount; i++)
	{
		unsigned int _Length;

		inFile.read((char*)&_Length, sizeof(_Length));
		char* _NewName = new char[_Length];
		inFile.read(_NewName, _Length);
		string _ShipName = _NewName;
		delete[] _NewName;

		inFile.read((char*)&_Length, sizeof(_Length));
		char* _NewClass = new char[_Length];
		inFile.read(_NewClass, _Length);
		string _Class = _NewClass;
		delete[] _NewClass;

		short _ShipLength;
		inFile.read((char*)&_ShipLength, sizeof(_ShipLength));

		int _Shield;
		inFile.read((char*)&_Shield, sizeof(_Shield));

		float _WarpSpeed;
		inFile.read((char*)&_WarpSpeed, sizeof(_WarpSpeed));


		unsigned int _WeaponCount;
		inFile.read((char*)&_WeaponCount, sizeof(_WeaponCount));

		vector<Ship::Weapons> WMD;
		int _FPower = 0;

		for (unsigned int j = 0; j < _WeaponCount; j++)
		{
			inFile.read((char*)&_Length, sizeof(_Length));
			char* _TName = new char[_Length];
			inFile.read(_TName, _Length);
			string _WName = _TName;
			delete[] _TName;

			int _Rating;
			inFile.read((char*)&_Rating, sizeof(_Rating));
			_FPower += _Rating;

			float _Power;
			inFile.read((char*)&_Power, sizeof(_Power));

			WMD.push_back(Ship::Weapons(_WName, _Rating, _Power));
		}

		Ships.push_back(Ship(_ShipName, _Class, _ShipLength, _Shield, _WarpSpeed, WMD, _FPower));
	}

} 

void Print(vector<Ship>& Ships) //Print the file
{
	for (unsigned int i = 0; i < Ships.size(); i++)
	{
		cout << "Name: " << Ships[i]._ShipName << endl;
		cout << "Class: " << Ships[i]._Class << endl;
		cout << "Length: " << Ships[i]._SLength << endl;
		cout << "Shield capacity: " << Ships[i]._Shield << endl;
		cout << "Maximum Warp: " << Ships[i]._Warp << endl;
		cout << "Armaments: " << endl;
		
		if (Ships[i].WMD.size() == 0)
		{
			cout << "Unarmed" << endl;
			cout << endl;
		}
		else
		{
			for (unsigned int j = 0; j < Ships[i].WMD.size(); j++)
			{
				cout << Ships[i].WMD[j]._WName << ", " << Ships[i].WMD[j]._Rating << ", " << Ships[i].WMD[j]._Power << endl;
			}
			cout << "Total firepower: " << Ships[i]._FPower << endl << endl;
		}
	}
}

void Strong(vector<Ship>& Ships) //Find the strongest ship in a file
{
	Ship Strongest = Ships[0];
	for (unsigned int i = 0; i < Ships.size(); i++)
	{
		if (Ships[i]._FPower > Strongest._FPower)
		{
			Strongest = Ships[i];
		}
	}

	cout << "Name: " << Strongest._ShipName << endl;
	cout << "Class: " << Strongest._Class << endl;
	cout << "Length: " << Strongest._SLength << endl;
	cout << "Shield capacity: " << Strongest._Shield << endl;
	cout << "Maximum Warp: " << Strongest._Warp << endl;
	cout << "Armaments: " << endl;

	if (Strongest.WMD.size() == 0)
	{
		cout << "Unarmed" << endl;
		cout << endl;
	}
	else
	{
		for (unsigned int j = 0; j < Strongest.WMD.size(); j++)
		{
			cout << Strongest.WMD[j]._WName << ", " << Strongest.WMD[j]._Rating << ", " << Strongest.WMD[j]._Power << endl;
		}
		cout << "Total firepower: " << Strongest._FPower << endl << endl;
	}
}

void StrongWMD(vector<Ship>& Ships) //Find the strongest ship weapon in a file
{
	Ship Strongest = Ships[0];
	int _Strongest = INT_MIN;

	for (unsigned int i = 0; i < Ships.size(); i++)
	{
		if (Ships[i].WMD.size() > 0)
		{
			for (unsigned int j = 0; j < Ships[i].WMD.size(); j++)
			{
				if (Ships[i].WMD[j]._Rating > _Strongest)
				{
					Strongest = Ships[i];
					_Strongest = Ships[i].WMD[j]._Rating;
				}
			}
		}
	}

	cout << "Name: " << Strongest._ShipName << endl;
	cout << "Class: " << Strongest._Class << endl;
	cout << "Length: " << Strongest._SLength << endl;
	cout << "Shield capacity: " << Strongest._Shield << endl;
	cout << "Maximum Warp: " << Strongest._Warp << endl;
	cout << "Armaments: " << endl;
	if (Strongest.WMD.size() == 0)
	{
		cout << "Unarmed" << endl;
		cout << endl;
	}

	else
	{
		for (unsigned int j = 0; j < Strongest.WMD.size(); j++)
		{
			cout << Strongest.WMD[j]._WName << ", " << Strongest.WMD[j]._Rating << ", " << Strongest.WMD[j]._Power << endl;
		}

		cout << "Total firepower: " << Strongest._FPower << endl << endl;
	}
}

void Weak(vector<Ship>& Ships) //Find the weakest ship in a file
{
	Ship Weakest = Ships[0];
	int _ShipCount = 1;

	while (Weakest._FPower == 0)
	{
		Weakest = Ships[_ShipCount];
		_ShipCount++;
	}

	for (unsigned int i = 0; i < Ships.size(); i++)
	{
		if (Ships[i].WMD.size() > 0)
		{
			for (unsigned int j = 0; j < Ships.size(); j++)
			{
				if (Ships[i]._FPower < Weakest._FPower)
				{
					Weakest = Ships[i];
				}
			}
		}
	}

	cout << "Name: " << Weakest._ShipName << endl;
	cout << "Class: " << Weakest._Class << endl;
	cout << "Length: " << Weakest._SLength << endl;
	cout << "Shield capacity: " << Weakest._Shield << endl;
	cout << "Maximum Warp: " << Weakest._Warp << endl;
	cout << "Armaments: " << endl;

	if (Weakest.WMD.size() == 0)
	{
		cout << "Unarmed" << endl;
		cout << endl;
	}
	else
	{
		for (unsigned int j = 0; j < Weakest.WMD.size(); j++)
		{
			cout << Weakest.WMD[j]._WName << ", " << Weakest.WMD[j]._Rating << ", " << Weakest.WMD[j]._Power << endl;
		}

		cout << "Total firepower: " << Weakest._FPower << endl << endl;
	}
}

void UnArmed(vector<Ship>& Ships) //Find the unarmed ships in a file. 
{
	for (unsigned int i = 0; i < Ships.size(); i++)
	{
		if (Ships[i].WMD.size() == 0)
		{
			cout << "Name: " << Ships[i]._ShipName << endl;
			cout << "Class: " << Ships[i]._Class << endl;
			cout << "Length: " << Ships[i]._SLength << endl;
			cout << "Shield capacity: " << Ships[i]._Shield << endl;
			cout << "Maximum Warp: " << Ships[i]._Warp << endl;
			cout << "Armaments: " << endl;
			cout << "Unarmed" << endl << endl;
		}
	}
}

//Lab 5 file for Daniel Laforce on 3/6/2022
//I tried moving all the functions and structs to the extra file and it keeps giving me an error. No matter how I include different files, it just causes more errrors. 
//Visual stuido keeps trying to change the include lines 
//BR1: Misspelled Armaments and didn't capitialize the W in Warp which caused half the tests to fail. 