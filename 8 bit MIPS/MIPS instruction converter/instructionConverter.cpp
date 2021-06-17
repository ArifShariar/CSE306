#include<bits/stdc++.h>
using namespace std;

string createOpcodeValue(string s){ //Returns opcode value with instruction type
    if(s=="j") return "0J";
    else if(s=="add") return "1R";
    else if(s=="or") return "2R";
    else if(s=="sll") return "3R";
    else if(s=="andi") return "4I";
    else if(s=="bneq" || s=="bne") return "5I";
    else if(s=="and") return "6R";
    else if(s=="ori") return "7I";
    else if(s=="srl") return "8R";/// double F in instructions set /// J for "srl" is missing. So using here
    else if(s=="beq") return "9I";
    else if(s=="lw") return "aI";
    else if(s=="subi") return "bI";
    else if(s=="sub") return "cR";
    else if(s=="sw") return "dI";
    else if(s=="nor") return "eR";
    else if(s=="addi") return "fI";
    else return "$x";
}

char createValue(string s){
    int pos = s.find(',');
    if(pos<s.size()) s.replace(pos,1,"");

    if(s=="$t0") return '0';
    else if(s=="$t1") return '1';
    else if(s=="$t2") return '2';
    else if(s=="$t3") return '3';
    else if(s=="$t4") return '4';
    else if(s=="$zero") return '5';
    else if(s=="$sp") return '6';
    else if(s=="$t5") return '7';
    else return '$';
}
string intToHexString(int val){
    string ret;
    stringstream hex_;
    hex_ << std::hex << val;
    hex_>>ret;
    return ret;
}

int main(){
    ifstream infile;
    infile.open("input.txt");
    ofstream outfile;
    outfile.open("output.txt");

    int line_count=-1;

    string operation,source1,source2,destination,shamt;
    int n[5],sum,instrucion_count=0,stack_memory=0,h;
    string str,word;
    vector<string>words,instructions; //stores words in a line
    vector<string> stack_instructions;

    map<string,int> mp;
    int flag;

    instructions.push_back("addi $sp, $zero, 255 "); ///stack initialization
    if(infile.is_open()){   ///need to read whole input to locate all branch labels
        while(getline(infile,str)){
            line_count++;
            if(str.size()<=1) continue;
            replace(str.begin(),str.end(),',',' ');
            instructions.push_back(str);

            int pos1 = str.find("push");
            int pos2 = str.find("pop");


            stringstream ss(str);
            while(ss>>word){
                if(word[0]=='/' && word[1]=='/') break;
                if(word[(int)word.size() -1]==':'){
                    word.replace((int)word.size() -1,1,"");
                    mp.insert(pair<string,int>(word,line_count)); //stores jump_label with it's line_number
                    break;
                }
                if(pos1>=0 && pos1<str.size()){
                    ss>>word;
                    int offset_pos = word.find('(');
                    if(offset_pos>=0 && offset_pos<str.size()){
                        instructions.push_back("lw $t5, "+word);
                        instructions.push_back("sw $t5, 0($sp)");
                    }else{
                        instructions.push_back("sw "+word+", 0($sp)");
                    }
                    instructions.push_back("subi $sp, $sp, 1");

                }else if(pos2>=0 && pos2<str.size()){
                    ss>>word;
                    instructions.push_back("addi $sp, $sp, 1");
                    instructions.push_back("lw "+word+", 0($sp)");
                }
            }
        }
        infile.close();
    }
    else{
        std::cout<<"Unable to open input file"<<endl;
    }
    line_count=-1;
     for(string s:instructions) {
         line_count++;
        stringstream ss(s);
        while(ss>>word){if(word[0]=='/' && word[1]=='/') break;words.push_back(word); } // cout<<endl;
        if(words.size()>=1){
            instrucion_count++;
            operation = createOpcodeValue(words[0]);
            if(operation[1]=='R'){
                destination = createValue(words[1]);
                source1 = createValue(words[2]);
                source2 = createValue(words[3]);

                if(source2=="$") {
                    stringstream int_(words[3]);
                    int_>>h;
                    source2 = intToHexString(h);
                }
                shamt ="0";
                if(operation[0]=='3' || operation[0]=='8') {shamt = source2; source2 = source1;source1="0";}
                //outfile<<words[0]<<" "<<words[1]<<" "<<words[2]<<" "<<words[3]<<endl;
                outfile<<operation[0]<<source1<<source2<<destination<<shamt<<" ";
            }
            else if(operation[1]=='I'){
                string src,dest,immd;
                int imd;
                dest = createValue(words[1]);
                if(operation[0]=='a' || operation[0]=='d') { /// lw/sw
                    int l = (int)words[2].find('(')+1;
                    src = createValue(words[2].substr(l,3));
                    immd = words[2].substr(0,l-1);
                    stringstream int_(immd);
                    int_>>imd;
                }else if(operation[0]=='5' || operation[0]=='9'){  ///if instruction is beq/bneq
                    src = dest;
                    dest = createValue(words[2]);
                    imd = mp[words[3]]-(line_count+1);
                }
                else{
                    src = createValue(words[2]); ///convert offset value to integer and stores in imd;
                    stringstream int_(words[3]);
                    int_>>imd;
                }
               ///converts integer imd to hex
                    immd = intToHexString(imd);
                    int len =immd.size();

                outfile<<operation[0]<<src<<dest;
                if(len>1) outfile<<immd[len-2]<< immd[len-1]<<" ";
                else outfile<<0<<immd[0]<<" ";

            }else if(operation[1]=='J'){
                int Target_Jump_Address = mp[words[1]];
                outfile<<operation[0];
                if(Target_Jump_Address<16) outfile<<"0"<< std::hex <<Target_Jump_Address; //converts integer Target_Jump_Address to hex
                else outfile<<std::hex <<Target_Jump_Address;
                outfile<<"00 ";
            }else if(words[0]=="push"){
                int offset_pos = words[1].find('(');
                if(offset_pos>=0 && offset_pos<str.size()) line_count-=3;
                else line_count-=2;

            }else if(words[0]=="pop") line_count-=2;
            else {
                if((line_count+1)<16) outfile<<"00"<< std::hex <<(line_count+1); ///converts integer (line_count+1) to hex
                else outfile<<0<<std::hex <<(line_count+1);
                outfile<<"00 ";
            }
        }
        words.clear();
    }


    if(instrucion_count<256)
        outfile<<256-instrucion_count<<"*0"<<endl;
    outfile.close();
}
