#include<bits/stdc++.h>
using namespace std;

int createOpcodeValue(string s){
    if(s=="j") return 0;
    else if(s=="add") return 1;
    else if(s=="or") return 2;
    else if(s=="sll") return 3;
    else if(s=="andi") return 4;
    else if(s=="bneq" || s=="bne") return 5;
    else if(s=="and") return 6;
    else if(s=="ori") return 7;
    else if(s=="srl") return 8;/// double F in instructions set /// J for "srl" is missing. So using here
    else if(s=="beq") return 9;
    else if(s=="lw") return 10;
    else if(s=="subi") return 11;
    else if(s=="sub") return 12;
    else if(s=="sw") return 13;
    else if(s=="nor") return 14;
    else if(s=="addi") return 15;
}

int createValue(string s){
    if(s=="$t0") return 0;
    else if(s=="$t1") return 1;
    else if(s=="$t2") return 2;
    else if(s=="$t3") return 3;
    else if(s=="$t4") return 4;
    else if(s=="$zero") return 5;
    else if(s=="$sp") return 6;
    else if(s=="$pc") return 7;
    else{
        stringstream print(s);
        int x=0;
        print >> x;
        return x;
    }
}

void convertToHex(int x){
    int rem,i=0;
    char hex[50];
    while(x!=0){
        rem=x%16;
        if(rem<10){
            rem=rem+48;
        }
        else{
            rem=rem+55;
        }
        hex[i]=rem;
        i++;
        x=x/16;
    }
    for(i=i-1;i>=0;i--){
        ofstream outfile;
        outfile.open("output306.TXT",ios_base::app);
        cout<<hex[i];
        outfile<<hex[i];

    }
    ofstream outfile;
    outfile.open("output306.TXT",ios_base::app);
    cout<<endl;
    outfile<<" ";
}

int main(){
    ifstream infile;
    string operation,source1,source2,destination,shamt;
    int n[5],sum,instrucion_count=0;
    string str,word;
    vector<string>v;
    infile.open("input306.TXT");
    if(infile.is_open()){
       ofstream outfile;
       outfile.open("output306.TXT");
       outfile<<"v2.0 raw"<<endl;
       while(getline(infile,str)){
          instrucion_count++;
          stringstream ss(str);

          ///add,sub,and,nor,sll,srl
          if((str.substr(0,3)=="add" || str.substr(0,3)=="sub"|| str.substr(0,3)=="and" || str.substr(0,3)=="nor"
             || str.substr(0,3)=="sll"|| str.substr(0,3)=="srl") && str[3]!='i'){
              //cout<<str<<endl;
              n[0]=createOpcodeValue(str.substr(0,3));/// for opcode
              str=str.substr(4,str.size());
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,',');
                  v.push_back(sub);
              }
              for(int i=0;i<v.size();i++){
                n[i+1]=createValue(v[i]);/// for registers
              }
              n[4]=0;///for shaft
              sum=0;
              sum=sum|n[4]|(n[1]<<4)|(n[3]<<8)|(n[2]<<12)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }

          ///or
          else if(str.substr(0,2)=="or" && str[2]!='i'){
              //cout<<str<<endl;
              n[0]=createOpcodeValue(str.substr(0,2));/// for opcode
              str=str.substr(3,str.size());
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,',');
                  v.push_back(sub);
              }
              for(int i=0;i<v.size();i++){
                n[i+1]=createValue(v[i]);/// for registers
              }
              n[4]=0;///for shaft
              sum=0;
              sum=sum|n[4]|(n[1]<<4)|(n[3]<<8)|(n[2]<<12)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }

          ///addi,subi,andi,bneq
          else if(str.substr(0,4)=="addi" || str.substr(0,4)=="subi"|| str.substr(0,4)=="andi" || str.substr(0,4)=="bneq"){
              //cout<<str<<endl;
              n[0]=createOpcodeValue(str.substr(0,4));/// for opcode
              str=str.substr(5,str.size());
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,',');
                  v.push_back(sub);
              }
              for(int i=0;i<v.size();i++){
                n[i+1]=createValue(v[i]);/// for registers
              }
              sum=0;
              sum=sum|n[3]|(n[1]<<8)|(n[2]<<12)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }

          ///ori,beq,bne
          else if(str.substr(0,3)=="ori" || str.substr(0,3)=="beq"|| str.substr(0,3)=="bne"){
              //cout<<str<<endl;
              n[0]=createOpcodeValue(str.substr(0,3));/// for opcode
              str=str.substr(4,str.size());
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,',');
                  v.push_back(sub);
              }
              for(int i=0;i<v.size();i++){
                n[i+1]=createValue(v[i]);/// for registers
              }
              sum=0;
              sum=sum|n[3]|(n[1]<<8)|(n[2]<<12)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }

          ///lw,sw
          else if(str.substr(0,2)=="lw" || str.substr(0,2)=="sw"){
              //cout<<str<<endl;
              n[0]=createOpcodeValue(str.substr(0,2));/// for opcode
              str=str.substr(3,str.size());
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,',');
                  v.push_back(sub);
              }
              for(int i=0;i<v.size();i++){
                n[i+1]=createValue(v[i]);/// for registers
              }
              n[3]=createValue(str.substr(str.size()-4,3));
              sum=0;
              sum=sum|n[2]|(n[1]<<8)|(n[3]<<12)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }

          ///j
          else if(str[0]=='j'){
              //cout<<str<<endl;
              stringstream s_stream(str);
              v.clear();
              while(s_stream.good()){
                  string sub;
                  getline(s_stream,sub,' ');
                  v.push_back(sub);
              }
              n[0]=createOpcodeValue(v[0]);
              n[1]=createValue(v[1]);
              sum=0;
              sum=sum|(n[1]<<8)|(n[0]<<16);
              //cout<<"Dec: "<<sum<<" ";
              convertToHex(sum);
          }
       }
    }
    else
        cout<<"Unable to open file"<<endl;

    ofstream outfile;
    outfile.open("output306.TXT",ios_base::app);
    outfile<<256-instrucion_count<<"*0"<<endl;
    infile.close();
}

