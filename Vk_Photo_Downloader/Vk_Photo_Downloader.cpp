﻿
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <boost/filesystem.hpp>
#include <urlmon.h>
#include <typeinfo>
#include <locale>

#pragma comment (lib, "urlmon.lib") // показываем компоновщику что будем использовать эту внешнюю библиотеку 

namespace fs = boost::filesystem;
using namespace std;

HRESULT DownloadFile(string line, fs::path current_dir, fs::path input_dir, string chat_name, string& photo_full_name, int& k) {
    int start = line.find("https://sun");
    int end = line.find("=album") + 6; 

    bool find_standart_dir = false;

    string link = line.substr(start, end - start);
    photo_full_name = input_dir.string();
    photo_full_name += "\\ALL_PHOTOS\\";
 
    if (current_dir.string().find("bookmarks") != std::string::npos)
    {
        
        photo_full_name +="\\BOOKMARKS\\"; 
        find_standart_dir = true;
    }
    if (current_dir.string().find("likes") != std::string::npos)
    {
        photo_full_name +="\\LIKES\\";
        find_standart_dir = true;
    }
    if (current_dir.string().find("messages") != std::string::npos)
    {
        photo_full_name += "\\MESSAGES\\";
        find_standart_dir = true;
    }
    if (current_dir.string().find("photos") != std::string::npos)
    {
        photo_full_name += "\\PHOTOS\\";
        find_standart_dir = true;
    }
    if (current_dir.string().find("profile") != std::string::npos)
    {
        photo_full_name += "\\PROFILE\\";
        find_standart_dir = true;
    }
    if (current_dir.string().find("video") != std::string::npos)
    {
        photo_full_name += "\\VIDEO\\";
        find_standart_dir = true;
    }
    if (current_dir.string().find("wall") != std::string::npos)
    {
        photo_full_name += "\\WALL\\";
        find_standart_dir = true;
    }
    if (!find_standart_dir)
    {
        photo_full_name += "\\OTHER\\";
    }
    photo_full_name = photo_full_name + chat_name + " (" + to_string(k) + ")" + ".jpg";
    HRESULT download_start = URLDownloadToFileA(0, link.c_str(), photo_full_name.c_str(), 0, 0);
    return download_start;
}

bool ReadFileHtml(fs::path current_dir, fs::path input_dir, int& k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    string line;
    string chat_name = " "; //default name
    string photo_full_name = " "; // для вывода ниже надо . надо ли 
    ifstream in(current_dir.string());
    int s;

    if (in.is_open())
    {
        while (getline(in, line))
        {
            if (line.find("<div class=\"ui_crumb\" >") != std::string::npos) // название чата всегда раньше самой переписки и фоток
            {

                int start_name = line.rfind("\" >") + 3; // начало имени
                int end_name = line.find("</div>", start_name - 1); // конец имени. Ищем div начиная с начала имени.

                chat_name = line.substr(start_name, end_name - start_name);
            }

            if (line.find("jpg") != std::string::npos)
            {
                if (line.find("https://sun") != std::string::npos)
                {
                    if (DownloadFile(line, current_dir, input_dir, chat_name, photo_full_name, k) == S_OK)
                    {
                        SetConsoleTextAttribute(hConsole, 10);
                        cout << "Photo number ";
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << k++;
                        SetConsoleTextAttribute(hConsole, 10);
                        cout << " is being downloaded from: ";
                        SetConsoleTextAttribute(hConsole, 11);
                        cout << chat_name;
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << " to: ";
                        SetConsoleTextAttribute(hConsole, 14);
                        cout << photo_full_name<<endl;
                        SetConsoleTextAttribute(hConsole, 15);
                        k++;
                    }
                }
            }
        }

    }

    in.close();
    return 0;
}


bool CreateDirectories(fs::path folder)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    string final_folder[9] = { "\\ALL_PHOTOS",
                               "\\BOOKMARKS",
                               "\\LIKES",
                               "\\MESSAGES",
                               "\\PHOTOS",
                               "\\PROFILE",
                               "\\VIDEO",
                               "\\WALL",
                               "\\OTHER"
                             };

   
    bool root_folder_exist = false;

    folder += final_folder[0]; 
    fs::path original_path = folder; // original path with \\ALL_PHOTOS

    for (int i = 0; i < 9; i++)
    {
        if (root_folder_exist)
        { 
            folder = original_path;
            folder += final_folder[i]; 
        }
        else 
        { 
            root_folder_exist = true;
        }

        if (!fs::is_directory(folder)) // если не существует папки ,то попробуем создать
        {
            if (fs::create_directory(folder))
            {
                SetConsoleTextAttribute(hConsole, 10);
                cout << "The " << folder << " folder has been successfully created\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else throw folder; 
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 14);
            cout << "The " << folder << " folder already exists\n";
            SetConsoleTextAttribute(hConsole, 15); 
        }

   }
    
    Sleep(1000);

    return 1;
}
int main()
{
   
    setlocale(LC_ALL, "RUS");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HWND hwnd = GetConsoleWindow(); // Узнаём сразу hwnd окна    
    MoveWindow(hwnd, 300, 200, 1300, 700, TRUE);//xnew,ynew,wnew,hnew -новые положение x,y, ширина и высота

    int k = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // for change console color
   //system("mode con cols=180 lines=50"); + чтобы можно было вверх вниз 
   
    //fs::path p("C:\\Users\\Alex\\Downloads\\Archive"); // стандартный путь к папке
    fs::path input_dir("C:\\Users\\Alex\\Downloads\\Archive");
    cout << "Enter the full path to the folder. "
        "Example: \"C:\\Users\\Alex\\Downloads\\Archive\" \n";
    
    string input;
    getline(cin, input);
    input_dir = input;
    cout <<"The entered directory: "<< input_dir << endl;
  
    try
    {
        CreateDirectories(input_dir); //p
    }
    catch (const fs::path& problematic_folder) // to save memory, we send it by link
    {
        SetConsoleTextAttribute(hConsole, 12);
        cerr << "Failed to create folder: " << problematic_folder << endl;
        cerr << "Try to run the program with administrator rights";
        SetConsoleTextAttribute(hConsole, 15);
        return 0;
    }
    catch (const exception& e)
    {
        SetConsoleTextAttribute(hConsole, 12);
        cerr << e.what();
        SetConsoleTextAttribute(hConsole, 15);
        exit(0);
    }

    fs::path current_dir;
    for (fs::recursive_directory_iterator i(input_dir), end; i != end; i++) // проход по всем файлам и папкам
    {
        if (!is_directory(i->path())) // если это файл
        {
            current_dir = i->path(); // ТУТ МЕНЯЕТ ИСХОДНЫЙ ПУТЬ
            //cout << "----------------"<<input_dir<<"++++++++++++++++++++";
            if (fs::path(current_dir.filename()).extension() == ".html")
            {
                ReadFileHtml(current_dir, input_dir, k);  
            }

        }
    }
    cout << "Total photos found: " << k;
}
