#include<bits/stdc++.h>
using namespace std;

string split(string s){
    string ret="";
    for(int i=0; i<s.length(); i++){
        if(s[i]==','){
            ret+=" ";
        }else if(s[i]=='x' || s[i]=='X'){
            if(s[i+1]!='o' && s[i+1]!='O'){
                continue;
            }
        }else{
            ret+=s[i];
        }
    }
    return ret;
}

class I{
    public :
    bitset<7> op;
    bitset<5> rd;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<12> imm;
    
I(string instr){
     instr=split(instr);
    istringstream iss(instr);
    string word ;
iss >> word;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    if(word=="lw"){
        op=0b0000011;
        func3=0b010;

        int n;
        iss >> n;
            rd=n;
        iss >> n;
            imm=n;
        iss >> n;
            rs1=n;

    }else{
        op=0b0010011;
        if(word=="addi"){
        func3= 0b000;
        }else if(word=="andi"){
        func3= 0b111;
        }else if(word=="ori"){
        func3= 0b110;
        }

        int n;
        iss >> n;
        rd=n;
        iss >> n;
            rs1=n;
        iss >> n;
            imm=n;
    }
    

}

string encode(){
    string s="";
    s+=imm.to_string(); 
    s+=rs1.to_string(); 
    s+=func3.to_string(); 
    s+=rd.to_string(); 
    s+=op.to_string();
    return s;
}

};

class R{
    public :
    bitset<7> op=0b0110011;
    bitset<5> rd;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<7> func7;
    
R(string instr){
     instr=split(instr);
    istringstream iss(instr);
    string word ;
iss >> word;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
     if(word=="sub"){
        func7=0b0100000;
        func3=0b000;
    }else{
        func7=0b0000000;
        if(word=="add"){
        func3= 0b000;
        }else if(word=="and"){
        func3= 0b111;
        }else if(word=="or"){
        func3= 0b110;
        }
    }
    
    int x;

iss >> x;
     rd=x;
iss >> x;
    rs1=x;
iss >> x;
     rs2=x;

}

string encode(){
    string s="";
    s+=func7.to_string(); 
    s+=rs2.to_string(); 
    s+=rs1.to_string(); 
    s+=func3.to_string(); 
    s+=rd.to_string(); 
    s+=op.to_string();
    return s;
}

};

class B{
    public :
    bitset<7> op=0b1100011;
    string imm1;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<5> rs2;
    string imm2="0";
    
B(string instr){
     instr=split(instr);
    istringstream iss(instr);
    string word ;
iss >> word;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    if(word=="beq"){
        func3=0b000;
    }else if(word=="bne"){
        func3=0b001;
    }else if(word=="blt"){
        func3=0b100;
    }
    int x;
iss >> x;
    rs1=x;
iss >> x;
      rs2=x;
iss >> x;
    bitset<12> imm;
    imm=x;
    string k=imm.to_string();
    imm2 += k.substr(1,6);
    imm1= k.substr(7,4);
    imm1 +=k.substr(0,1);
  // imm2= [12| 10:5 ]    imm1=[4:1 | 11];   
  //VERY IMP NOTE : i am considering that imm=[11:0] 
  // Since in case of branchs and jumps, imm is always a multiple of 4 so last bit that is 0th bit of imm is 0
  // i am letting the 12 th bit in imm2 to be 0;
  // also when i will simulate i will assemble these bits and shift left by 1 so that i get the same imm 
  // then i will divide it by four to get the no. of instructions to jump ahead because my memory will be an array so no need to be a multiple of 4
  //hare krishna 
}

string encode(){
    string s="";
    s+=imm2;
    s+=rs2.to_string(); 
    s+=rs1.to_string(); 
    s+=func3.to_string(); 
    s+=imm1;
    s+=op.to_string();
    return s;
}

};

class J{
    public :
    bitset<7> op;
    bitset<5> rd;
    bitset<20> imm;
J(string instr){
     instr=split(instr);
    istringstream iss(instr);
    string word ;
iss >> word;
if(word=="jal"){
    op=0b1101111;
}
int x;

iss >> x;
     rd=x;
iss >> x;
     imm=x;
// here also immediate will be a multiple of 4 only ; just like branch
}

string encode(){
    string s="";
    s+=imm.to_string(); 
    s+=rd.to_string(); 
    s+=op.to_string();
    return s;
}

};

class S{
    public :
    bitset<7> op;
    string imm1;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<5> rs2;
    string imm2;
    
S(string instr){
    instr=split(instr);
    istringstream iss(instr);
    string word ;
iss >> word;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    if(word=="sw"){
        op=0b0100011;
        func3=0b010;
    }
    int x;
iss >> x;
     rs2=x;
iss >> x;
    bitset<12> imm;
    imm=x;
    string k=imm.to_string();
    imm2= k.substr(0,7);
    imm1= k.substr(7,5);
iss >> x;
     rs1=x;

}

string encode(){
    string s="";
    s+=imm2;
    s+=rs2.to_string(); 
    s+=rs1.to_string(); 
    s+=func3.to_string(); 
    s+=imm1;
    s+=op.to_string();
    return s;
}

};

int main(){
    ifstream RVcode("input.txt");
    ofstream output("binary_encoded.txt");
    string line;
     while(getline(RVcode,line)){
         istringstream iss(line);
         string word ;
         iss >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
         if(word =="lw" || word =="addi" || word =="andi" || word =="ori" ){
            I instr(line); 
            output<<instr.encode()<<endl;
         }else 
         if(word =="add" || word =="sub" || word =="and" || word =="or" ){
            R instr(line); 
            output<<instr.encode()<<endl;
         }else 
         if(word =="beq" || word =="bne" || word =="blt"){
            B instr(line); 
            output<<instr.encode()<<endl;
         }else 
         if(word =="jal"  ){
            J instr(line);  
            output<<instr.encode()<<endl;
         }else 
         if(word =="sw" ){
            S instr(line); 
            output<<instr.encode()<<endl;
         }
        
        

        iss.clear();    
        iss.seekg(0); 

        }
     
output.close();  
RVcode.close();
}