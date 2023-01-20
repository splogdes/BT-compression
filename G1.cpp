#include <iostream>
#include <vector>
#include <string>
 
struct BNode{
    std::string val;
    BNode* left;
    BNode* right;
};

BNode* bt_node(std::string e,BNode* l,BNode* r){
    BNode* tmp = new BNode;
    tmp -> val = e;
    tmp -> left = l;
    tmp -> right = r;
    return tmp;
}

// once a zero is hit it builds the branch off the tree in that location
BNode* hit_0(std::string const direction,std::vector<int> const ord,int counter){
    std::string name;
    char x01 = direction[ord[counter]];
    name = "x" + std::to_string(ord[counter] + 1);
    if((x01 == 'x')||(ord[counter]==-1)){
        return bt_node("1",NULL,NULL);
    }
    if(x01 == '1'){
        return bt_node(name,bt_node("0",NULL,NULL),hit_0(direction,ord,counter+1));
    }
    return bt_node(name,hit_0(direction,ord,counter+1),bt_node("0",NULL,NULL));
}

// moves through the existing tree until a zero is hit
BNode* Build_tree(std::string const direction,std::vector<int> const ord,BNode* t,int counter){
    char x01 = direction[ord[counter]];
    if(t -> val != "0"){
        if(x01=='0'){
        t -> left = Build_tree(direction,ord,t->left,counter+1);
        return t;
        }
        if(x01=='1'){
            t -> right = Build_tree(direction,ord,t->right,counter+1);
            return t;
        }
    }
    return hit_0(direction,ord,counter);
}

// works out the optimal order of the varibles in the tree
std::vector<int> order(std::vector<std::string> in){
    std::vector <int> rank;
    std::vector<int> out;
    for(int i=0;i<in[0].size();i++){
        int counter = 0;
        for(int j=0;j<in.size();j++){
            if(in[j][i]=='x'){
                counter++;
            }
        }
        rank.push_back(counter);
    }
    for(int a = 0; a < in[0].size();a++){
        int min = -1, track;
        for(int k=0;k<rank.size();k++){
            if(((rank[k] < min)&&(rank[k]!=-1))||(min==-1)){
                min = rank[k];
                track = k;
            }
        }
        rank[track]=-1;
        out.push_back(track);
    }
    out.push_back(-1);
    return out;
}

//this compresses the inputs with "don't cares as in the report"
std::vector<std::string> shorthand(std::vector<std::string> r){
    std::vector<std::string> tmp;
        for(int y=r[0].size()-1; y>=0;y--){
            int i=0;
            while((i<r.size())){
                int j = i+1,check = 1;
                while((j < r.size())&&(check!=-1)){
                    int x =0;
                    check=-1;
                    while((x != r[0].size())&&(check == -1)){
                        if((r[i][x]!=r[j][x])&&(x!=y)){
                            check = 0;
                        }
                        x++;
                    }
                    if(check == -1){
                        r[i][y]='x';
                        r[j]="";
                    }
                    j++;
                    if(r[j]==""){
                        j++;
                    }
                }
                tmp.push_back(r[i]);
                i++;
                if(r[i]==""){
                    i++;
                }
            }
            r = tmp;
            tmp.clear();
        }
    return r;
}

//builds the compressed tree
BNode* build_bt(const std::vector<std::string>& fvalues){
    std::vector<int> first;
    BNode* t;
    t = bt_node("0",NULL,NULL);
    if (fvalues.size() > 0){
        std::vector<std::string> r = shorthand(fvalues);
        first = order(r);
        for(int i=0;i<r.size();i++){
            t=Build_tree(r[i],first,t,0);
        }
    }
    return t;
}
 
// do not alter the following function
// this function converts e.g. std::string "x3" to int 2
int label_to_idx(const std::string& label){
 
    std::string out;
 
    for(int i = 1; i < label.size(); i++){
        out.push_back(label[i]);
    }
 
    return std::stoi(out) - 1;
}

void destroy_tree(BNode* T){
  if(T != NULL){
    destroy_tree(T->left);
    destroy_tree(T->right);
    delete T;
  }
}

// do not alter the following function
std::string eval_bt(BNode* bt, const std::string& input){
 
    if( (bt->left == NULL) && (bt->right == NULL) ){
        return bt->val;
    }
    else{
        int idx = label_to_idx(bt->val);
        std::string input_idx;
        input_idx.push_back(input[idx]);
 
        if(input_idx == "0"){
            return eval_bt(bt->left, input); 
        }
        else{
            return eval_bt(bt->right, input); 
        }
    }
}
 
// do not alter the following function
int n_nodes_bt(BNode* t){
    if(t == NULL){
        return 0;
    }
    else{
        return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
    }
}
 
class BoolTree{
public:
 
    BoolTree(const std::vector<std::string>& fvalues){
       t = build_bt(fvalues);
    }
 
    std::string eval(const std::string& s){
        return eval_bt(t, s);
    }
 
    int n_nodes(){
        return n_nodes_bt(t);
    }
 
    ~BoolTree(){
        destroy_tree(t);
    }
 
private:
    BNode* t;
};
 
// the main provided below must work correctly
// with your implementation for the code above
// however you can change it as you wish for your own testing 
// and your changes won't be considered for the assessment
// (in your submission you can also have an empty main or no main at all)
 
int main(){
 
    std::vector<std::string> fv;
    std::string row;
 
    row = "11";
    fv.push_back(row);
 
    BoolTree ft1(fv);
    // as in the second assignment we give as input only the rows
    // of the truth table whose value is 1
    // (this is an example with the boolean "and" function)
 
    fv.clear();
 
    row = "010";
    fv.push_back(row);
    row = "011";
    fv.push_back(row);
    row = "110";
    fv.push_back(row);
    row = "111";
    fv.push_back(row);
 
    BoolTree ft2(fv);
    // this corresponds to the f(x1, x2, x3) example shown above
 
    std::cout << ft1.n_nodes() << std::endl;
    // we expect this to print 5
 
    std::cout << ft2.n_nodes() << std::endl;
    // if the algorithm is such that it builds the smallest possible corresponding tree
    // for the boolean function we are considering
    // then this will print 3 (see tree diagram in the example above)
 
    std::cout << ft1.eval("01") << std::endl;
    // this should print "0" 
 
    std::cout << ft1.eval("11") << std::endl;
    // this should print "1"
 
    std::cout << ft2.eval("001") << std::endl;
    // this should print "0"
 
    std::cout << ft2.eval("110") << std::endl;
    // this should print "1"
}