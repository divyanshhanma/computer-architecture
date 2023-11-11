#include<bits/stdc++.h>
using namespace std;
int x[32] = {0};        // register memory
int mem[1001] = {0};    // data memory
bool block[32]={false};
int id[32]={0};     // false--you can take , true--you have to wait
//string s == instruction 

typedef struct IFID{
  int dpc;
  string ir;

}IFID;

typedef struct IDEX{
  int dpc;
  //int jpc;
  map<string,bool> cw;
  int rs1;
  int rs2;
  int imm;
  int rdl;
  string function; // contains whether to add, sub,sddi,etc;
}IDEX; 

typedef struct EXMO{
    int dpc;
    map<string,bool> cw;
    int ALUout;
    int rs2;
    int rdl;
}EXMO;

typedef struct MOWB{
    int dpc;
    map<string,bool> cw;
    int ALUout;
    int LDout;
    int rdl;
}MOWB;

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
int ALU(int a, int b,string x){

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

bool stall(string s){
     bitset<5> b1(s.substr(12,5));
    bitset<5> b2(s.substr(7,5));
    unsigned int rsl1=b1.to_ulong();
    unsigned int rsl2=b2.to_ulong();

    if(block[rsl1] || block[rsl2]){
        return true;
    }
    return false;
}

// return the array index of destination reg
int  rdl_generator(string s){
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

    IFID ifid;
    IDEX idex;
    EXMO exmo;
    MOWB mowb;


    bool ifidStall = false;
    bool idexStall = false;
    bool exmoStall = false;
    bool mowbStall = false;
    bool finalStall =false;
    bool ifidValid = false;
    bool idexValid = false;
    bool exmoValid = false;
    bool mowbValid = false;

    
    int lo= (v.size() + 4);

    while(i<lo ){
         
         if(!finalStall && mowbValid){
        
            if(mowb.cw["MemRead"]){
              x[mowb.rdl]=mowb.LDout;
              if(id[mowb.rdl]==mowb.dpc){
                block[mowb.rdl]=false;}
             
            }else if(!mowb.cw["MemWrite"]){
              x[mowb.rdl]=mowb.ALUout;
              if(id[mowb.rdl]==mowb.dpc){
                block[mowb.rdl]=false;}
            }
         }
       
        if(!mowbStall && exmoValid){
            //MOWB modification and Memory unit funtions performed
            mowb.dpc=exmo.dpc;
       mowb.cw = exmo.cw;
       if(exmo.cw["MemRead"]){
        mowb.LDout = mem[exmo.ALUout];
       }
       mowb.ALUout=exmo.ALUout;
       mowb.rdl=exmo.rdl;

       if(exmo.cw["MemWrite"]){
        mem[exmo.ALUout]=exmo.rs2;
       }
       mowbValid=true;
       //--
        }else{
            mowbValid=false;
        }
        
        if(!exmoStall && idexValid){
           exmo.dpc=idex.dpc;
            //EXMO modification and execution unit funtions performed
       exmo.cw=idex.cw;
       if(idex.cw["ALUsrc"]){
            exmo.ALUout=ALU(idex.rs1, idex.imm, idex.function );  
       }else{
            exmo.ALUout=ALU(idex.rs1, idex.rs2, idex.function );
       }

        if(exmo.cw["isBranch"] && flag(v[exmo.dpc],exmo.ALUout)){
            i=idex.dpc+idex.imm;
            exmoValid=false;
            idexValid=false;
            ifidValid=false;
            continue;
        }
        if(exmo.cw["isJump"]){ 
             i=idex.dpc+idex.imm;
            exmoValid=false;
            idexValid=false;
            ifidValid=false;
            continue;
          }

       exmo.rs2=idex.rs2;
       exmo.rdl=idex.rdl;
       idexStall=false;
       exmoValid=true;
        //--
        }else{
            exmoValid=false;
        }
    
       if(!idexStall && ifidValid){
      
         //IDEX modification and decode unit funtions performed
         if(stall(ifid.ir)){
            ifidStall=true;
            idexValid=false;
            continue;
       }
          
           
       idex.dpc=ifid.dpc;
       idex.imm=imm_genertor(ifid.ir);
       idex.cw=controller(ifid.ir);
       pair<int,int> p=rs1_rs2_generator(ifid.ir);
       idex.rs1=p.first;
       idex.rs2=p.second;
       idex.function=ALUselect(ifid.ir);
       idex.rdl=rdl_generator(ifid.ir);

        string opc=ifid.ir.substr(25,7);

       if(opc=="0010011" || opc=="0000011" || opc=="0110011"){
        block[idex.rdl]=true;
        id[idex.rdl]=idex.dpc;
       }
          ifidStall=false;
          idexValid=true;
        //--
       }else{
        idexValid=false;
       }
       
        if(!ifidStall){
           
             //IFID modification
        if(i<v.size()){
         ifid.ir=v[i];  
         ifid.dpc=i;    
         ifidValid=true;
        //--
        }else{
            ifidValid=false;
        }
            i++; 
         
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



