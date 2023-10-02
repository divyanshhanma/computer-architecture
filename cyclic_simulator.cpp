#include<bits/stdc++.h>
using namespace std;
int x[32] = {0};      // register memory
int mem[1001] = {0};    // data memory
//string s == instruction 

// returns whether add,and,or,sub;
string ALUselect(string s){
    string func3 = s.substr(17,3);
    
 /*I*/   if(s.substr(25,7)=="0010011"){
        if(func3=="000") return "add";
        else if(func3=="110") return "or";
        else if(func3=="111") return "and";
 /*R*/   }else if(s.substr(25,7)=="0110011"){
         if(s.substr(0,7)=="0000000"){
            if(func3=="000") return "add";
            else if(func3=="110") return "or";
            else if(func3=="111") return "and";
         }else{
             if(func3=="000") return "sub";
         }
 /*B*/   }else if(s.substr(25,7)=="1100011"){
            return "sub";
        }
 /*L*/    else if(s.substr(25,7)=="0000011"){
           return "add";
        }
 /*S*/   else if(s.substr(25,7)=="0100011"){
            return "add";
        }
    
    
}
  
// return int result; a==input1, b==input2
int ALU(int a, int b,string s){
string x=ALUselect(s);

if(x=="add") return (a+b);
else if(x=="or")  return (a|b);
else if(x=="and") return (a&b);
else if(x=="sub") return (a-b);
}

//pair== rs1,rs2;   returns values stored in Rs1 and Rs2
pair<int,int> rs1_rs2_generator(string s){
    bitset<5> b1(s.substr(12,5));
    bitset<5> b2(s.substr(7,5));
    unsigned int rsl1=b1.to_ulong();
    unsigned int rsl2=b2.to_ulong();
    pair<int,int> p={x[rsl1],x[rsl2]};
    return p;
}

// return the array index of destination reg
unsigned int  rdl_generator(string s){
     bitset<5> d(s.substr(20,5));
     unsigned int rdl=d.to_ulong();
     return rdl;
}

// generates the decimal value of imm i.e, for J,s,l and B it returns after dividing by 4
int imm_genertor(string s){

 /*for I type*/     if(s.substr(25,7)=="0010011" || s.substr(25,7)=="0000011"  ){
                        bitset<12> bits(s.substr(0,12)); 
                        int decimal = 0;
                        int sign = bits[bits.size() - 1]; 

                        for (int i = 0; i < bits.size() - 1; i++) {
                        decimal += bits[i] * (1 << i);
                        }

                        if (sign) {
                        decimal = -((1 << (bits.size() - 1)) - decimal);
                        }
                        if(s.substr(25,7)=="0000011") {return decimal/4;}
                        else return decimal;
                    }
 /*for B type*/     else if(s.substr(25,7)=="1100011"){
                        string imm = s.substr(0,1);
                        imm = imm + s.substr(24,1) + s.substr(1,6) + s.substr(20,4);

                        bitset<12> bits(imm);
                        bits <<=1;   // left shift by 1;
                        int decimal = 0;
                        int sign = bits[bits.size() - 1]; 

                        for (int i = 0; i < bits.size() - 1; i++) {
                        decimal += bits[i] * (1 << i);
                        }

                        if (sign) {
                        decimal = -((1 << (bits.size() - 1)) - decimal);
                        }
                        return decimal/4;
                        
                    }
 /*for J type*/     else if(s.substr(25,7)=="1101111"){
                         bitset<20> bits(s.substr(0,20)); 
                        int decimal = 0;
                        int sign = bits[bits.size() - 1]; 

                        for (int i = 0; i < bits.size() - 1; i++) {
                        decimal += bits[i] * (1 << i);
                        }

                        if (sign) {
                        decimal = -((1 << (bits.size() - 1)) - decimal);
                        }
                        return decimal/4;
                    }
 /*for S type*/     else if(s.substr(25,7)=="0100011"){
                        string imm2=s.substr(0,7);
                        string imm1=s.substr(20,5);
                        string imm = imm2 + imm1;
                         bitset<12> bits(imm); 
                        int decimal = 0;
                        int sign = bits[bits.size() - 1]; 

                        for (int i = 0; i < bits.size() - 1; i++) {
                        decimal += bits[i] * (1 << i);
                        }

                        if (sign) {
                        decimal = -((1 << (bits.size() - 1)) - decimal);
                        }
                        return decimal/4;
                    }

}

bool flag(string s,int x){
        string func3 = s.substr(17,3);
      /*beq*/  if(func3=="000" && x==0){return true;}
      /*bne*/  else if(func3=="001" && x!=0){return true;}
      /*blt*/  else if(func3=="100" && x<0){return true;}
      return false;
}

// generates control word
map<string,bool> controller(string s){
   
   map<string,bool> cw;
   cw["ALUsrc"]=false;
   cw["isBranch"]=false;
   cw["isJump"]=false;
   cw["MemRead"]=false;  //isLoad
   cw["MemWrite"]=false;  //isStore

    if(s.substr(25,7)=="0010011" || s.substr(25,7)=="0000011" || s.substr(25,7)=="0100011"  ){
        cw["ALUsrc"]= true;
    }
    if(s.substr(25,7)=="1100011"){
        cw["isBranch"]= true;
    }
    if(s.substr(25,7)=="1101111"){
        cw["isJump"]=true;
    }
    if(s.substr(25,7)=="0000011"){
      cw["MemRead"]=true;   
    }
    if(s.substr(25,7)=="0100011"){
        cw["MemWrite"]= true;
    }
    return cw;
}

int main(){
    vector<string> v;
    ifstream bincode("binary_encoded.txt");
    ofstream reg("registers.txt");
    ofstream M("Memory.txt");
    string line;
    while(getline(bincode,line)){
          v.push_back(line);
    }
    bincode.close();
    int i=0;
    while(i!=v.size()){
        
        int imm = imm_genertor(v[i]); 
        int rdl = rdl_generator(v[i]);  
        map<string,bool> control_word= controller(v[i]);
        if(control_word["isJump"]){ x[rdl]=i+1;  i+=imm; continue;  } //jump

        pair<int,int> i1_i2 = rs1_rs2_generator(v[i]);
        int Alu_result;
       

        if(control_word["ALUsrc"]){
            Alu_result=ALU(i1_i2.first, imm, v[i] );  
        }else{
            Alu_result=ALU(i1_i2.first, i1_i2.second, v[i] );
        }
            
        if(control_word["isBranch"] && flag(v[i],Alu_result)){ //branch
             i+=imm; continue;
            
        }
        if(control_word["MemWrite"]){  // store  
            mem[Alu_result] = i1_i2.second;   
            i++; 
            continue;
        }
        if(control_word["MemRead"]){ //load
            x[rdl] = mem[Alu_result];
            i++;  
            continue;
        }
        else{
            x[rdl]=Alu_result;
            i++; 
            continue;
        }
    }
  
    for(int i=0; i<32; i++){
    reg<<"x"<<i<<" : "<<x[i]<<endl;
    }

    for(int i=0; i<1001; i++){
    M<<"mem["<<i<<"] : "<<mem[i]<<endl;
    }

    reg.close();
    M.close();
    return 0;
}