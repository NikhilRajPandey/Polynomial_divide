#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <ctype.h>
#include <cstdlib>
#include <sstream>

/* Note: I have writed some code in comments for debugging purpose */ 
struct Term{
    char variable;
    int var_pow;
    int coefficient;
    std::string give_in_string();
};
std::string Term::give_in_string(){
    std::string returning_;
    if(this->coefficient > 0){
        returning_.append("+");
    }

    returning_.append(std::to_string(this->coefficient));
    if(this->var_pow !=0){
        returning_.append(1,this->variable);
        if(this->var_pow > 1){
            returning_.append("^");
            returning_.append(std::to_string(this->var_pow));
        }
    }
    return returning_;
}

template<typename Custo>
void print_arr(Custo param,int Arrsize){
    std::cout<<"[";
    for (int i = 0; i < Arrsize-1; i++)
    {
        std::cout<<'"'<<param[i]<<'"'<<",";
    }
    std::cout<<'"'<<param[Arrsize-1]<<'"';
    std::cout<<"]"<<std::endl;
}
class Poly{
    private:
        bool is_sign(const char &param_char){
            if(param_char == '+' || param_char == '-'){return true;}
            return false;
        }
        std::vector<std::string> split_eq(std::string param_poly){
            // It will seperate all the terms from a equation/Polynomial
            std::vector<std::string> returning_;
            // Split by +,-
            if(!Poly::is_sign(param_poly.at(0))){
                param_poly.insert(0,"+"); // Since no sign its means it is positive
            }
            char curr_alpha;
            std::string curr_term = std::string(1,param_poly.at(0));
            for (int index = 1; index < param_poly.length(); index++){
                curr_alpha = param_poly.at(index);
                if(Poly::is_sign(curr_alpha)){
                    returning_.push_back(curr_term);
                    curr_term = "";
                }
                curr_term.push_back(curr_alpha);
            }
            returning_.push_back(curr_term);
            return returning_;  
        }
        int conv_into_num(std::string param_string){
            std::stringstream into_number(param_string);
            int number;
            into_number>>number;
            return number;
        }
        bool is_number(const std::string& s){
            for (int i = 0; i < s.size(); i++){
                if(!isdigit(s.at(i))){return false;}
            }
            return true;         
        }

        void seprate_term_parts(std::vector<Term> &param_address,const std::vector<std::string> &param_terms){
 
            for (int inde = 0; inde < param_terms.size(); inde++){
                param_address.push_back(Term());
                std::string curr_term = param_terms.at(inde);
                int sign = 1;
                if(curr_term.at(0) == '-'){sign = -1;}
                curr_term = curr_term.substr(1,curr_term.size());
                param_address.at(inde).variable = this->variable;

                // Finding degree/power of the term
                if(curr_term.find('^') == -1 && this->is_number(curr_term)){
                    // if this condition followed then its a constant
                    param_address.at(inde).coefficient = this->conv_into_num(curr_term) * sign;
                    param_address.at(inde).var_pow = 0;
                    continue;
                }
                std::string power;
                int pow_index = curr_term.size()-1; // Starting from last
                // This will give the '^' index from last from term part like(4x^23)
                char curElOEl; // Means Current Element of Element

                while (pow_index >= 0){
                    curElOEl = curr_term.at(pow_index);
                    if(curElOEl == '^'){ // Means It have power more than 1
                        param_address.at(inde).var_pow = this->conv_into_num(power);
                        break;    
                    }
                    else if(curElOEl == this->variable){ // Means It have power 1
                        param_address.at(inde).var_pow = 1;
                        break;
                    }
                    power.insert(0,1,curElOEl);
                    pow_index--;
                }

                // Finding the coefficient
                param_address.at(inde).coefficient = 1;
                /*By default it will be 1 for terms like x, x^2 .. You Know What I mean*/
                while (pow_index >=0){ // I Will start after where i found the power
                    curElOEl = curr_term.at(pow_index);
                    if(isdigit(curElOEl)){
                        param_address.at(inde).coefficient = this->conv_into_num(curr_term.substr(0,pow_index+1)); 
                        break;
                    }
                    pow_index--;
                }
                param_address.at(inde).coefficient = param_address.at(inde).coefficient * sign;
            } 
        }
        void print_terms(){
            for (int i = 0; i < this->term_sp_divisor.size(); i++)
            {
                std::cout<<term_sp_divisor.at(i).coefficient<<std::endl;
                std::cout<<term_sp_divisor.at(i).var_pow<<std::endl;
                std::cout<<term_sp_divisor.at(i).variable<<std::endl;
            }
            
        }
        int give_max(int inde ,const std::vector<Term> &equation_addr,const int &size_term){
            // This function will give me the minimum degree term index
            // inde will be the starting index from where i have to start tranvsersing
            int min_inde = inde;
            inde++;
            while(inde < size_term){
                Term curr_element = equation_addr.at(inde);
                if(curr_element.var_pow > equation_addr.at(min_inde).var_pow){
                    min_inde = inde;
                }
                inde++;
            }
            return min_inde;
        }
        int give_same_power_term(Term &matching_term){
            // std::cout<<"Eroor"<<std::endl;
            int size_dividend = this->term_sp_dividend.size();
            int matchin_inde = 0; // Not Defined

            // std::cout<<"\nMatchin_term: "<<matching_term.give_in_string()<<" "<<matching_term.var_pow<<std::endl;

            for (int inde = 0; inde < size_dividend; inde++){

                // std::cout<<"Term: "<<term_sp_dividend.at(inde).give_in_string()<<" "<<term_sp_dividend.at(inde).var_pow<<std::endl;

                if(this->term_sp_dividend.at(inde).var_pow == matching_term.var_pow){
                    return inde;
                }
                // If our matching_term is greater than the current term means that i will not find any maching_term cause the terms are in standard form/sorted by their power
                /* Reference: https://www.geeksforgeeks.org/adding-two-polynomials-using-linked-list/ */
                else if(matching_term.var_pow > this->term_sp_dividend.at(inde).var_pow){

                    // std::cout<<"True"<<std::endl;
                    matchin_inde = inde - 1;
                    break;
                }
            }
            // std::cout<<"\nTerm Not Found"<<std::endl;

            // Means the all the term in dividend are smaller than matching_term or the size is 0 in dividend
            // and the same power term not found then its time to create one
            Term default_term;
            default_term.var_pow = matching_term.var_pow;
            default_term.coefficient = 0;
            default_term.variable = this->variable;

            // Now Seeing Where I have to add this term
            if(size_dividend == 0 or matchin_inde == -1){matchin_inde = 0;}
            this->term_sp_dividend.insert(this->term_sp_dividend.begin()+matchin_inde,default_term);
            return matchin_inde;
        }
    public:
        char variable;
        std::vector<Term> term_sp_divisor; // Term Seperated Divisor
        std::vector<Term> term_sp_dividend; // Term Seperated Dividend

        Poly(std::string divisor,std::string dividend,char param_variable);
        // Notice Your Equation should be in the standard form if it is not then call conv_stand
        std::vector<std::string> divide();
        bool max_poly();
        void conv_standard(std::vector<Term> &equation_address);
        std::string build_equation_from_terms(std::vector<Term> &param_equation);
};

Poly::Poly(std::string divisor,std::string dividend,char param_variable){
    this->variable = param_variable;
    this->seprate_term_parts(this->term_sp_divisor,this->split_eq(divisor));
    this->seprate_term_parts(this->term_sp_dividend,this->split_eq(dividend));
    this->conv_standard(this->term_sp_divisor);
    this->conv_standard(this->term_sp_dividend);

}
void Poly::conv_standard(std::vector<Term> &equation_address){
    // This will sort the term in terms of degrees/power
    int size_of_addr = equation_address.size();
    for (int inde = 0; inde < equation_address.size(); inde++){
        Term cache = equation_address.at(inde);
        int min_inde = this->give_max(inde,equation_address,size_of_addr);
        // Swaping two elements for sorting
        equation_address.at(inde) = equation_address.at(min_inde);
        equation_address.at(min_inde) = cache;
    }
}
std::vector<std::string> Poly::divide(){
    // This will return {quoitent,reminder}
    
    std::vector<Term> quiotent; // Quoitent will be made up of multiple term like 4x+2
    /* Now implementing the Normal Divison Algorithm that is shown in X Class Math Book */
    // Read max_poly code
    std::vector<Term> &divisor = this->term_sp_divisor;
    std::vector<Term> &dividend = this->term_sp_dividend;
    int divisor_degree = this->term_sp_divisor.at(0).var_pow;
    int nOfDivsTerm = this->term_sp_divisor.size();

    Term quoitent_term;

    while(this->max_poly()){
        if(dividend.at(0).coefficient % divisor.at(0).coefficient!=0){
            perror("Currently We don't Support a fraction quoitent");
            break;
        }
        else{
            quoitent_term.coefficient = dividend.at(0).coefficient/divisor.at(0).coefficient;
            quoitent_term.variable = this->variable;
            quoitent_term.var_pow = dividend.at(0).var_pow - divisor_degree;
            dividend.erase(dividend.begin());
            quiotent.push_back(quoitent_term);

            // Iterating all the factors that i could create from divisor and then subtracting through dividend
            for (int n_trav = 1; n_trav < nOfDivsTerm; n_trav++){
            // n_trav means no of traveled
                Term faOfDvTe; // Means Factor of divsion term
                faOfDvTe.variable = this->variable;
                faOfDvTe.var_pow = quoitent_term.var_pow+divisor.at(n_trav).var_pow;
                faOfDvTe.coefficient = quoitent_term.coefficient * divisor.at(n_trav).coefficient;

                int matching_term_index = this->give_same_power_term(faOfDvTe);
                // Now Subtracting
                dividend.at(matching_term_index).coefficient = dividend.at(matching_term_index).coefficient - faOfDvTe.coefficient;

                if(dividend.at(matching_term_index).coefficient == 0){
                    dividend.erase(dividend.begin()+matching_term_index);
                }
            }
        }
    }
    // The terms that function not divided it will be in the dividend that will be our reminder
    if(dividend.size() == 0){ // Means reminder is 0
        return {this->build_equation_from_terms(quiotent),"0"};
    }
    return {this->build_equation_from_terms(quiotent),this->build_equation_from_terms(dividend)};
    
}
std::string Poly::build_equation_from_terms(std::vector<Term> &param_equation){
    // It will just join all the terms and give the quation
    std::string returning_;
    for (int i = 0; i < param_equation.size(); i++){
        returning_.append(param_equation.at(i).give_in_string());
    }
    return returning_;
    
}
bool Poly::max_poly(){
    /* Give you true if dividend degree is greater or equal to divisor */
    if(this->term_sp_dividend.size() > 0){ // Other Wise Reminder is 0
        if(this->term_sp_dividend.at(0).var_pow >= this->term_sp_divisor.at(0).var_pow){
            return true;
        }
    }
    return false;
}
    

int main(){
    // Poly testing_("-x^2+x-1","-x^3+3x^2-3x+5",'x');
    /* All the testing sets */
    // Poly testing_("x^2-2","2x^4-3x^3-3x^2+6x-2",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;
    // Poly testing_("x^2-2","x^3-3x^2+5x-3",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;
    // Poly testing_("2-x^2","x^4-5x+6",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;
    // Poly testing_("x^2-2","x^4",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;
    // Poly testing_("x^2-2","x^4+x^3",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;
    Poly testing_("x+1","x^3+1",'x');
    // std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;

    // Returns quoitent,reminder
    std::vector<std::string> divison_answer = testing_.divide();
    std::cout<<divison_answer.at(0)<<" "<<divison_answer.at(1)<<std::endl;

    return 0;
}
