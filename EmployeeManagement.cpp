	#include<iostream>
#include<fstream>											//For File Operations.
#include<sstream>										//For Istringstream functionality.
#include<vector>											//For vector operations.
#include<cstdlib>											//For exit() function.
#include<string>											//For String declearation and operations.
#include<climits>
#include<iomanip>

using namespace std;

const int EMP_ID_WIDTH = 8;
const int EMP_NAME_WIDTH = 20;
const int EMP_EMAIL_WIDTH = 25;
const int EMP_NUMERIC_WIDTH = 10;
const int TOTAL_WIDTH = 100;

struct Employee
{
	int empId;
	string name;
	double basicSalary;
	double pf;
	double healthInsAmt;
	string email;
};

void readFile(vector <Employee>& v_emp);
void printMenu();
void doTask(vector <Employee>& v_emp, int option);
void addEmployee(vector <Employee>& v_emp);
int searchEmployee(vector <Employee> v_emp, int targetEmpId);
void printEmployees(vector <Employee> v_emp);
void printEmployee(Employee e);
double getNetSalary(Employee e);
void saveToFile(vector <Employee> v_emp);
bool deleteEmployee(vector <Employee>& v_emp,int targetEmp);

void readFile(vector <Employee>& v_emp)
{
		ifstream fin("/home/yash/Documents/CPP_Practice/emp.txt");
		if (!fin){
			cout<<"Unable to open emp.txt. File not found. \n";
			exit(1);
		}
		string line;
		int recNo = 0;
		while(!fin.eof()){
			std::getline(fin, line);
			recNo ++;
			//cout<<recNo<<" - "<<line<<"\n";
			istringstream iss(line);
			string strEmpId;
			string strName;
			string strBasicSalary;
			string strPf;
			string strHealthInsAmt;
			string strEmail;
			std::getline(iss,  strEmpId, ',');
			std::getline(iss,  strName, ',');
			std::getline(iss, strBasicSalary, ',');
			std::getline(iss,  strPf, ',');
			std::getline(iss,  strHealthInsAmt, ',');
			std::getline(iss,  strEmail, ',');
			
			Employee temp;
			temp.empId = atoi(strEmpId.c_str());
			temp.name = strName;
			temp.basicSalary = atof(strBasicSalary.c_str());
			temp.pf = atof(strPf.c_str());
			temp.healthInsAmt = atof(strHealthInsAmt.c_str());
			temp.email = strEmail;
			
			v_emp.push_back(temp);
		}
		fin.close();
}

void printMenu()
{
	cout<<"1. Add Employee. \n";
	cout<<"2. Print Employee report. \n";
	cout<<"3. Search Employee\n";
	cout<<"4. Delete Employee \n";
	cout<<"5. Save \n";
	cout<<"6. Exit \n";
}

int searchEmployee(vector <Employee> v_emp, int targetEmpId)
{
	for(int i = 0; i < v_emp.size(); i++){
		if(v_emp[i].empId == targetEmpId){
			return i;
		}
	}
	return -1;
}

void addEmployee(vector <Employee>& v_emp)
{
	Employee temp;
	bool isDuplicate = false;
	do{
		cout<<"Emp Id: ";
		cin>>temp.empId;
		isDuplicate = false;
		if (searchEmployee(v_emp, temp.empId) != -1){
			isDuplicate = true;
			cout<<"Employee Id: "<<temp.empId<<" already exists, please input unique Id.\n";
		}
	}while(isDuplicate);
	cout<<"Name: ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	std::getline(cin,temp.name);
	cout<<"Basic Salary($): ";
	cin>>temp.basicSalary;
	cout<<"PF($): ";
	cin>>temp.pf;
	cout<<"Health Ins($): ";
	cin>>temp.healthInsAmt;
	cout<<"Email: ";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	std::getline(cin,temp.email);
	
	v_emp.push_back(temp);
	cout<<"Employee with id: "<<temp.empId<<" added successfully.\n";
	cout<<"Total employees: "<<v_emp.size()<<"\n\n";
}

void doTask(vector <Employee>& v_emp, int option)
{
	switch(option){
		case 1:  addEmployee(v_emp);
					break;
		case 2: printEmployees(v_emp);
					break;
		case 3:  int targetEmpId;
					cout<<"Please enter the id of the employee to be searched.: ";
					cin>>targetEmpId;
					searchEmployee(v_emp, targetEmpId);
					if(searchEmployee(v_emp, targetEmpId) == -1){
						cout<<"Employee does not exist.\n";
						break;
					}
					else{
						cout<<"Employee found at index :"<<searchEmployee(v_emp, targetEmpId)<<"\n";
						break;
					}
					break;
		case 4: //int targetEmpId;
					cout<<"Please enter the employee id to delete.: ";
					cin>>targetEmpId;
					if(deleteEmployee(v_emp, targetEmpId)){
						cout<<"The employee has been deleted successfully.\n";
					}
					else{
						cout<<"The targeted employee could not be deleted.\n";
					}
					break;
		case 5: saveToFile(v_emp);
					break;
		default: cout<<"Invalid option. Options are from(1 to 6).\n";
	}
}	

void printEmployees(vector <Employee> v_emp)
{
	cout<<"\n";
	cout << setw(EMP_ID_WIDTH) << left << "EmpId"
		   << setw(EMP_NAME_WIDTH) << left << "Name"
		   << setw(EMP_EMAIL_WIDTH) << left << "Email"
		   << setw(EMP_NUMERIC_WIDTH) << right << "Baic ($)"
		   << setw(EMP_NUMERIC_WIDTH) << right << "PF ($)"
		   << setw(EMP_NUMERIC_WIDTH) << right << "HltIns ($)"
		   << setw(EMP_NUMERIC_WIDTH) << right << "Net ($)\n";
		   cout << setw(TOTAL_WIDTH) << setfill('-') << " " << endl;
		   cout << setfill(' ');
		   double totalBasic  = 0.0;
		   double totalPfDeduction = 0.0;
		   double totalHealthIns = 0.0;
		   double totalNetSalary = 0.0;
		   
		   for (vector <Employee>::iterator it = v_emp.begin();it != v_emp.end();it++){
			   printEmployee(*it);
			   
			   totalBasic += it->basicSalary;
			   totalPfDeduction += it->pf;
			   totalHealthIns += it->healthInsAmt;
			   totalNetSalary += getNetSalary(*it);
		   }
		   cout << setw(TOTAL_WIDTH) << setfill('-') << " " << endl;
		   cout << setfill(' ') ;
		   
		   cout << setw(EMP_ID_WIDTH) << left<< "Total"
				  << setw(EMP_NAME_WIDTH) << left << "In ($)"
				  << setw(EMP_EMAIL_WIDTH) << " "
				  << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalBasic
				  << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalPfDeduction
				  << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalHealthIns
				  << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalNetSalary << "\n" ;
}

void printEmployee(Employee e)
{
	cout << setw(EMP_ID_WIDTH) << left << e.empId
		   << setw(EMP_NAME_WIDTH) << left << e.name
		   << setw(EMP_EMAIL_WIDTH) << left << e.email
		   << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.basicSalary
		   << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.pf
		   << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.healthInsAmt
		   << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << getNetSalary(e) << endl;
}

double getNetSalary(Employee e)
{
	return e.basicSalary - (e.pf + e.healthInsAmt);
}

void saveToFile(vector <Employee> v_emp)
{
	ofstream fout("emp.txt");
	if(!fout){
		cout<<"Unable to open the data file emp.txt\n";
		return;
	}
	int recCount = 0;
	for(vector <Employee>::iterator it = v_emp.begin();it != v_emp.end();it++){
		fout<<it->empId<<","<<it->name<<","<<it->basicSalary<<","
			  <<it->pf<<","<<it->healthInsAmt<<","<<it->email;
		recCount++;
		if(recCount != v_emp.size()){
			fout<<endl;
		}
	}
	fout.close();
}

bool deleteEmployee(vector <Employee>& v_emp,int  targetEmp)
{
	int index = searchEmployee(v_emp,targetEmp);
	if(index == -1){
		return false;
	}
	
	cout<<"Employee with id: "<<targetEmp<<" found at index "<<index<<"\n";
	int option;
	cout<<"Are you sure to delete the employee? Input 1 to delete 0 to exit.\n";
	cin>>option;
	if(option == 1){
		v_emp.erase(v_emp.begin() + index);
		return true;
	}
	return false;
}

int main()
{
	vector <Employee> v_emp;
	readFile(v_emp);
	cout<<"Total "<<v_emp.size() <<" records read from the data file\n"; 
	printMenu();
		bool quit = false;
		while(!quit){
			cout<<"Please input your option(1 - 6): ";
			int option;
			cin>>option;
			if(option == 6){
				quit = true;
			}
			else{
				doTask(v_emp, option);
			}
		}
	return 0;
}