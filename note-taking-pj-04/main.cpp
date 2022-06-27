#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class Image {

};

class Note {
	string _id;
	string _title;
	string _text;
	bool bType = false;
	vector<Image*> _images;
public:
	Note(){}

	Note(string id, string title, string text)
	{
		_id = id;
		_title = title;
		_text = text;
	}

	~Note() {}

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

class ILocalPersistence {
protected:
	vector<Note*> _noteList;
public:
	ILocalPersistence() {}
	~ILocalPersistence(){}
	vector<Note*> getNotes() { return _noteList; }
	virtual void saveNote(Note* note){}
	virtual void updateNote (string id) {}
	virtual void deleteNote (string id) {}
	virtual void DisplayNotes() {}
};

class FileManager : public ILocalPersistence {
	ifstream _fileIn;
	ofstream _fileOut;
	string _name;
	string _password;
	bool bCheck = false;
public:

	void setName(string *name) { _name = *name; }
	string getName() { return _name; }
	void setPassword(string *password) { _password = *password; }
	string getPassword() { return _password; }

	bool InPut() {
		_fileIn.open("Info.txt");
		while (_fileIn.peek() != ifstream::traits_type::eof()) {
			getline(_fileIn, _name);
			getline(_fileIn, _password);
			if (_name != "" && _password != "") {
				bCheck = true;
			}
			_fileIn.ignore();
		}
		_fileIn.close();		
		return bCheck;
	}

	void OutPut() {
		_fileOut.open("Info.txt");		
		_fileOut << _name << "\n" << _password << endl;
	}

	virtual void saveNote(Note* note)
	{
		_noteList.push_back(note);
	}

	virtual void UpdateNote(string id)
	{
		
	}

	virtual void deleteNote(string id)
	{

	}

	virtual void DisplayNotes()
	{
		if (_noteList.size() != 0)

			for (int i = 0; i < _noteList.size(); i++)
				cout << "" << _noteList[i]->getID() << " | " << _noteList[i]->getTitle() << "\n" << "--------" << _noteList[i]->getText() << "\n";
		else
			cout << "Add a note" << "\n";
		
	}
};

class NetworkController {
	vector<Note*> _noteList;

public:
	vector<Note*> getNote() { return _noteList; }
	void create(Note *note)
	{
		_noteList.push_back(note);
	};
	void updateNote(Note note) {};
	void deleteNote(Note note) {};
};


class PrivateNote : public Note {
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
	UI(){}
	~UI()
	{
		//delete fManager;
		delete note;
		delete network;
	}

	void Title(string title)
	{
		cout << "== " << title << " ==" << "\n";
	}

	string InputDate()
	{
		string text, title, time;
		cout << "Enter note Title: ";
		getline(cin, title);
		cout << "Enter note Text: " << "\n";
		getline(cin, text);

		return title + "|" + text;
	}

	void Header(FileManager* fManager)
	{
		
		cout << "Name: " << fManager->getName() << "\n" << "Note Number: " << fManager->getNotes().size() << "\n\n\n";
	}

	void DisplayNoteMenu(FileManager* fManager)
	{
		system("cls");
		Header(fManager);

		// Display Note List
		Title("Note List");		
		fManager->DisplayNotes();

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
		{	
			string title, text, inputString;
			inputString = InputDate();
			for (int i = 0; i < inputString.length(); i++)
			{
				if (inputString[i] != '|')
					title = title + inputString[i];
				else
				{
					for (int j = i + 1; j < inputString.length(); j++)
						text = text + inputString[j];
					break;
				}
			}
			note = new Note(to_string(fManager->getNotes().size() + 1),title, text);		
			network->create(note);
			fManager->saveNote(note);
			break;
		}
		case'2':
		{
			break;
		}
		case'S':
		case's':
			break;
		case'B':
		case'b':
			break;
		case'E':
		case'e':
			exit(1);
			break;
		default:
			cout << "wrong Choice" << endl;
		}
	}

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

		switch(choice[0])
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
				DisplayNoteMenu(fManager);
				break;
			}
			case'2':
			{
				// Private note list
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
				//Exit
				exit(1);				
			}
				
			default:
				cout << "wrong Choice" << endl;
			}
		}
	}

};

int main() {
	FileManager* fManager = new FileManager();
	UI* ui = new UI();
	string name, password;
	if (!fManager->InPut()) {
		cout << "Enter your name" << endl;
		getline(cin, name);
		cout << "Enter Privet Notes Password" << endl;
		getline(cin, password);
		fManager->setName(&name);
		fManager->setPassword(&password);
		fManager->OutPut();
	}else
	{		
		ui->DisplayMenu(fManager);
	}

	delete ui;
	delete fManager;

	return 0;
}