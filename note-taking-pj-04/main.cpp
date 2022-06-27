#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class Image
{
};

class Note
{
	string _id;
	string _title;
	string _text;
	bool bType;
	vector<Image*> _images;
public:
	Note()
	{
	}

	Note(string id, string title, string text, bool bType = false)
	{
		_id = id;
		_title = title;
		_text = text;
		this->bType = bType;
	}

	~Note()
	{
	}

	void setID(string id) { _id = id; }
	string getID() { return _id; }
	void setTitle(string title) { _title = title; }
	string getTitle() { return _title; }
	void setText(string text) { _text = text; }
	string getText() { return _text; }
	void addImages(Image image) { _images.push_back(&image); }
	vector<Image*> getImages() { return _images; }
	void setbType(bool btype) { bType = btype; }
	bool getbType() { return bType; }
};

class ILocalPersistence
{
protected:
	vector<Note*> _noteList;
	vector<Note*> _pNoteList;
public:
	ILocalPersistence()
	{
	}

	~ILocalPersistence()
	{
	}

	vector<Note*> getNotes() { return _noteList; }
	vector<Note*> getPrivatNotes() { return _pNoteList; }

	virtual void SaveNote(Note* note, bool bType)
	{
	}

	virtual void UpdateNote(string id, bool bType)
	{
	}

	virtual void DeleteNote(string id, bool bType)
	{
	}

	virtual void DisplayNotes()
	{
	}

	virtual void DisplayPrivateNotes()
	{
		
	}

	virtual void DisplayNote(string id)
	{
	}
};

class FileManager : public ILocalPersistence
{
	ifstream _fileIn;
	ofstream _fileOut;
	string _name;
	string _password;
	bool bCheck = false;
public:
	void setName(string* name) { _name = *name; }
	string getName() { return _name; }
	void setPassword(string* password) { _password = *password; }
	string getPassword() { return _password; }

	bool InPut()
	{
		_fileIn.open("Info.txt");
		while (_fileIn.peek() != ifstream::traits_type::eof())
		{
			getline(_fileIn, _name);
			getline(_fileIn, _password);
			if (_name != "" && _password != "")
			{
				bCheck = true;
			}
			_fileIn.ignore();
		}
		_fileIn.close();
		return bCheck;
	}

	void OutPut()
	{
		_fileOut.open("Info.txt");
		_fileOut << _name << "\n" << _password << endl;
	}

	void SaveNote(Note* note, bool bType) override
	{
		if(bType)
			_pNoteList.push_back(note);
		else
			_noteList.push_back(note);
	}

	virtual void UpdateNote(Note* note, string id, string title, string text, bool bType)
	{
		if(!bType)
			for (int i = 0; i < _pNoteList.size(); i++)
				if (id == _pNoteList[i]->getID())
					_pNoteList[i] = note;
				else
					cout << "ID" << "'" << id << "'" << "Does Not Exist" << "\n";
		else
			for (int i = 0; i < _noteList.size(); i++)
				if (id == _noteList[i]->getID())
					_noteList[i] = note;
				else
					cout << "ID" << "'" << id << "'" << "Does Not Exist" << "\n";
	}

	void DeleteNote(string id, bool bType) override
	{
		if(bType)
			_noteList.erase(remove(_pNoteList.begin(), _pNoteList.end(), _pNoteList[stoi(id)]), _pNoteList.end());
		else
			_noteList.erase(remove(_noteList.begin(), _noteList.end(), _noteList[stoi(id)]), _noteList.end());
	}

	void DisplayNotes() override
	{
		if (_noteList.size() != 0)

			for (int i = 0; i < _noteList.size(); i++)
				cout << "" << _noteList[i]->getID() << " | " << _noteList[i]->getTitle() << "\n" << "--------" <<
					_noteList[i]->getText() << "\n";
		else
			cout << "Add a note" << "\n";
	}

	void DisplayPrivateNotes() override
	{
		if (_pNoteList.size() != 0)

			for (int i = 0; i < _pNoteList.size(); i++)
				cout << "" << _pNoteList[i]->getID() << " | " << _pNoteList[i]->getTitle() << "\n" << "--------" <<
				_pNoteList[i]->getText() << "\n";
		else
			cout << "Add a note" << "\n";
	}

	virtual void DisplayNote(int id)
	{
		cout << "" << _noteList[id]->getID() << " | " << _noteList[id]->getTitle() << "\n" << "--------" << _noteList[
			id]->getText() << "\n";
	}
};

class NetworkController
{
	vector<Note*> _noteList;

public:
	vector<Note*> getNote() { return _noteList; }

	void create(Note* note)
	{
		_noteList.push_back(note);
	};

	void updateNote(Note note)
	{
	};

	void deleteNote(Note note)
	{
	};
};


class PrivateNote : public Note
{
	string _passwordHash;
public:
	void setText(string text) { _passwordHash = text; }
	string getText() { return _passwordHash; }
};

class UI
{
	//FileManager* fManager = new FileManager();
	NetworkController* network = new NetworkController();
	Note* note;
public:
	UI()
	{
	}

	~UI()
	{
		//delete fManager;
		delete note;
		delete network;
	}

	// Helpful Functions
	void Title(string title)
	{
		cout << "== " << title << " ==" << "\n";
	}

	string InputData(string prifix)
	{
		string text, title, time;
		cout << prifix << " note Title: ";
		getline(cin, title);
		cout << prifix << " note Text: " << "\n";
		getline(cin, text);

		return title + "|" + text;
	}

	void Header(FileManager* fManager)
	{
		cout << "Name: " << fManager->getName() << "\n" << "Note Number: " << fManager->getNotes().size() << "\n\n\n";
	}

	// Note Functions
	void AddNote(FileManager* fManager, bool bType)
	{
		string id, inputData, choice, title, text;
		inputData = InputData("Enter");
		for (int i = 0; i < inputData.length(); i++)
		{
			if (inputData[i] != '|')
				title = title + inputData[i];
			else
			{
				for (int j = i + 1; j < inputData.length(); j++)
					text = text + inputData[j];
				break;
			}
		}

		note = new Note(bType ? to_string(fManager->getPrivatNotes().size() + 1) : to_string(fManager->getNotes().size() + 1), title, text, bType);
		/*cout << "haddy type: " << (bType ? (to_string(fManager->getPrivatNotes().size() + 1 ) + " Private Notes")  : (to_string((fManager->getNotes().size() + 1) ) + " Notes")) << endl;
		cin.ignore();*/
		network->create(note);
		fManager->SaveNote(note, bType);
		DisplayNoteMenu(fManager, bType);
	}

	void UpdateNote(FileManager* fManager, bool bType)
	{
		string id, inputData, choice, title, text;
		cout << "Enter an id: ";
		getline(cin, id);
		
		if (stoi(id) >= 0 && stoi(id) < (bType ? fManager->getPrivatNotes().size() : fManager->getNotes().size()))
		{
			inputData = InputData("Update");
			for (int i = 0; i < inputData.length(); i++)
			{
				if (inputData[i] != '|')
					title = title + inputData[i];
				else
				{
					for (int j = i + 1; j < inputData.length(); j++)
						text = text + inputData[j];
					break;
				}
			}
			note = new Note(id, title, text, bType);
			fManager->UpdateNote(note, id, title, text, bType);

		}
		else
		{
			cout << "ID" << "'" << id << "'" << "Does Not Exist" << "\n";
			cout << "Try Again (y/n): " << "\n";
			getline(cin, choice);
			if (choice[0] == 'y' || choice[0] == 'Y')
				UpdateNote(fManager, bType);
			
		}
		DisplayNoteMenu(fManager, bType);

	}

	void DeleteNote(FileManager* fManager, bool bType)
	{
		string id, inputData, choice, title, text;
		cout << "Enter an id: ";
		getline(cin, id);
		id = to_string(stoi(id) - 1);

		if (stoi(id) >= 0 && stoi(id) < bType ? fManager->getPrivatNotes().size() : fManager->getNotes().size())
			fManager->DeleteNote(id, bType);
		else
		{
			cout << "ID" << "'" << id << "'" << "Does Not Exist" << "\n";
			cout << "Try Again (y/n): " << "\n";
			getline(cin, choice);
			if (choice[0] == 'y' || choice[0] == 'Y')
				DeleteNote(fManager, bType);
			
		}
		DisplayNoteMenu(fManager, bType);

	}

	void DisplayNoteMenu(FileManager* fManager, bool bType)
	{
		system("cls");
		Header(fManager);

		// Display Note List
		if(bType)
		{
			Title("Private Notes List");
			fManager->DisplayPrivateNotes();
		}
		else
		{
			Title("Notes List");
			fManager->DisplayNotes();
		}
			

		// Display Note Menu
		Title("Menu");
		cout << "Add Note ___________________(1)" << "\n";
		cout << "Update Note ________________(2)" << "\n";
		cout << "Delete Note ________________(3)" << "\n";
		cout << "Back To Main Menu __________(B)" << "\n";
		cout << "Exit _______________________(E)" << "\n\n";
		cout << "Select: ";

		string choice;

		getline(cin, choice);
		cin.clear();

		switch (choice[0])
		{
		case '1':

			AddNote(fManager, bType);
			break;

		case'2':

			UpdateNote(fManager, bType);
			break;

		case'3':

			DeleteNote(fManager, bType);
			break;

		case'B':
		case'b':
			DisplayMenu(fManager);
			break;
		case'E':
		case'e':
			exit(1);
			break;
		default:
			cout << "wrong Choice" << endl;
		}
	}

	// Settings Functions
	void ChangePassword(FileManager* fManager)
	{
		string password, choice;
		cout << "Enter The Old Password: ";
		getline(cin, password);
		cin.clear();

		if (password == fManager->getPassword())
		{
			cout << "Enter a New Password: ";
			getline(cin, password);
			cin.clear();

			fManager->setPassword(&password);
			fManager->OutPut();
		}
		else
		{
			cout << "Password Incorrect!" << "\n";
			cout << "Try Again (y/n): " << "\n";
			getline(cin, choice);
			if (choice[0] == 'y' || choice[0] == 'Y')
				ChangePassword(fManager);
			else
				DisplaySettingMenu(fManager);
		}
	}

	void DisplaySettingMenu(FileManager* fManager)
	{
		system("cls");
		Header(fManager);
		Title("Settings Menu");
		cout << "Change Name ________________(1)" << "\n";
		cout << "Change Password ____________(2)" << "\n";
		cout << "Back To Main Menu __________(B)" << "\n";
		cout << "Exit _______________________(E)" << "\n\n";
		cout << "Select: ";

		string choice;
		getline(cin, choice);
		cin.clear();

		switch (choice[0])
		{
		case '1':
			{
				string name;
				cout << "Enter New Name: ";
				getline(cin, name);
				cin.clear();
				fManager->setName(&name);
				fManager->OutPut();

				break;
			}
		case '2':
			ChangePassword(fManager);
			break;

		case 'B':
		case 'b':
			DisplayMenu(fManager);
			break;

		case 'E':
		case 'e':
			exit(1);

		default:
			cout << "wrong Choice" << endl;
		}
	}

	// Private Note FUnctions
	void PasswordDisplay(FileManager* fManager)
	{
		string password, choice;
		cout << "Enter Password: ";
		getline(cin, password);
		cin.clear();
		if (fManager->getPassword() == password)
			DisplayNoteMenu(fManager, true);
		else
		{
			cout << "Try Again (y/n): " << "\n";
			getline(cin, choice);
			if (choice[0] == 'y' || choice[0] == 'Y')
				PasswordDisplay(fManager);
			else
				DisplayMenu(fManager);
		}
	}

	// Main Menu
	void DisplayMenu(FileManager* fManager)
	{
		bool bBreak = true;
		while (bBreak)
		{
			system("cls");
			Header(fManager);
			Title("Menu");
			cout << "Note List __________________(1)" << "\n";
			cout << "Private Note List __________(2)" << "\n";
			cout << "Settings ___________________(S)" << "\n";
			cout << "Exit _______________________(E)" << "\n\n";
			cout << "Select: ";

			string choice;
			getline(cin, choice);
			cin.clear();

			switch (choice[0])
			{
			case '1':
				{
					// Display Note					
					DisplayNoteMenu(fManager, false);
					break;
				}
			case'2':
				{
					// Display Private Note
					PasswordDisplay(fManager);
					break;
				}
			case'S':
			case's':
				{
					// Settings
					DisplaySettingMenu(fManager);
					break;
				}
			case'E':
			case'e':
				{
					// Exit
					exit(1);
				}

			default:
				cout << "wrong Choice" << endl;
			}
		}
	}
};

int main()
{
	auto fManager = new FileManager();
	auto ui = new UI();
	string name, password;
	if (!fManager->InPut())
	{
		cout << "Enter your name" << endl;
		getline(cin, name);
		cout << "Enter Privet Notes Password" << endl;
		getline(cin, password);
		fManager->setName(&name);
		fManager->setPassword(&password);
		fManager->OutPut();
	}
	else
	{
		ui->DisplayMenu(fManager);
	}

	delete ui;
	delete fManager;

	return 0;
}
