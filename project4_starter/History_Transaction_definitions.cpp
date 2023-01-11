#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol,
                         unsigned int day_date,
                         unsigned int month_date,
                         unsigned int year_date,
                         bool buy_sell_trans,
                         unsigned int number_shares,
                         double trans_amount)
{
  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  p_next = nullptr;
  if (buy_sell_trans == true)
    trans_type = "Buy";
  else
    trans_type = "Sell";
  shares = number_shares;
  amount = trans_amount;
  trans_id = assigned_trans_id;
  assigned_trans_id++;
  acb=0;
  acb_per_share = 0;
  cgl = 0;
  share_balance = 0;
}

// Destructor
// TASK 1
//
Transaction::~Transaction() {}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other)
{
  if (this->year < other.get_year())
    {
    return true;}
    
    else if ((this->year == other.get_year()) && (this->month < other.get_month()))
    {
    return true;}

    else if ((this->year == other.get_year()) && (this->month == other.get_month()) && (this->day < other.get_day()))
    {
    return true;}

    else if ((this->year == other.get_year()) && (this->month == other.get_month()) && (this->day == other.get_day()) && (this->trans_id > other.get_trans_id()))
    {
    return true;}
       
  return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }
void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }
void Transaction::set_cgl(double value) { cgl = value; }
void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print()
{
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
            << std::setw(4) << get_symbol() << " "
            << std::setw(4) << get_day() << " "
            << std::setw(4) << get_month() << " "
            << std::setw(4) << get_year() << " ";

  if (get_trans_type())
  {
    std::cout << "  Buy  ";
  }
  else
  {
    std::cout << "  Sell ";
  }

  std::cout << std::setw(4) << get_shares() << " "
            << std::setw(10) << get_amount() << " "
            << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
            << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
            << std::setw(10) << std::setprecision(3) << get_cgl()
            << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 3
//
History::History()
{
  p_head = nullptr;
}

// Destructor
// TASK 3
//
History::~History()
{
  Transaction *p_current = p_head;
  while (p_current != nullptr)
  {
    Transaction *next = p_current->get_next();
    delete p_current;
    p_current = next;
  }


}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history()
{
  ece150::open_file();
  while (ece150::next_trans_entry())
  {
    insert(new Transaction (ece150::get_trans_symbol(),
                            ece150::get_trans_day(),
                            ece150::get_trans_month(),
                            ece150::get_trans_year(),
                            ece150::get_trans_type(),
                            ece150::get_trans_shares(),  
                            ece150::get_trans_amount()  
                           )
          );
  }
  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans)
{
  if (p_head == nullptr)
  {
    p_head = p_new_trans;
  }
  else
  {
    Transaction* current = p_head;
    while (current->get_next()!= nullptr)
    {
      current = current -> get_next();
    }
    current->set_next(p_new_trans);
  }

}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date()
{
  //Declare a new list, let something point to p_head, set previous and after
  Transaction* newList = nullptr;
  Transaction* head = new Transaction("ticker_symbol",0,0,0,0,0,0);
  Transaction* prev = p_head;
  Transaction* after = p_head;

  //Entering the linked list
  while(p_head != nullptr)
  {
    //set a current minimum and a temp that loops through the list
    Transaction* current_min = p_head;
    head->set_next(p_head);
    Transaction* temp = head;

    while (temp->get_next() != nullptr)
    {
      if(*temp->get_next() < *current_min)
      {
        current_min = temp->get_next();
        after = temp->get_next()->get_next();
        prev = temp;
      }
      temp = temp->get_next();
    }
    if (current_min == p_head)
    {
      p_head = p_head->get_next();
      head->set_next(p_head);
    }
    else
    {
      prev->set_next(after);
    }
    current_min->set_next(nullptr);

    //Append the minimum to the sorted list
    
   
    // if (newList == nullptr)
    // {
    //   newList = current_min;
    // }
    // else
    // {
    //   Transaction* newListCounter = newList;
    //   while(newListCounter->get_next() != nullptr)
    //   {
    //     newListCounter = newListCounter->get_next();
    //   }
    //   newListCounter->set_next(current_min);
    // }

    Transaction* newListCounter = newList;
    while(true)
    {
      if(newList == nullptr)
      {
        newList = current_min;
        break;
      }
      else
      {
        if (newListCounter->get_next() != nullptr)
        {
          newListCounter = newListCounter->get_next();
        }
        else
        {
          newListCounter->set_next(current_min);
          break;
        }
      }
    }
  }
  p_head = newList;
  delete head;
  // head = nullptr;

}



// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl()
{
  Transaction* temp = p_head;
  /*
  while (temp != nullptr)
  {
    //if true, trans type = buy

     if(temp->get_trans_type())
      {
        temp->set_acb(temp->get_acb() + temp->get_amount());
        temp->set_share_balance(temp->get_share_balance() + temp->get_shares());
        temp->set_acb_per_share(temp->get_acb()/temp->get_share_balance());
      }
      //else indicates the trans type = sell
      else  
      {
        temp->set_acb(temp->get_acb()-(temp->get_shares()*temp->get_acb_per_share()));
        temp->set_share_balance(temp->get_share_balance() - temp->get_shares());
        temp->set_acb_per_share(temp->get_acb()/temp->get_share_balance());
        temp->set_cgl(temp->get_amount() - (temp->get_shares() * temp->get_acb_per_share()));
      }
  }*/

  double acb1 = 0;
  double share_balance1 = 0;
  double acb_per_share1 = 0;

  while (temp != nullptr)
  {
    //if true, trans type = buy
    if (temp->get_trans_type())
    {
      temp->set_acb(acb1 + temp->get_amount());
      acb1 = temp->get_acb();
      temp->set_share_balance(share_balance1 + temp->get_shares());
      share_balance1 = temp->get_share_balance();
      temp->set_acb_per_share(acb1/share_balance1);
      acb_per_share1 = temp->get_acb_per_share();
    }
    //else indicates the trans type = sell
    else
    {
      temp->set_acb(acb1 - (temp->get_shares()*acb_per_share1));
      acb1 = temp->get_acb();
      temp->set_share_balance(share_balance1 - temp->get_shares());
      share_balance1 = temp->get_share_balance();
      temp->set_acb_per_share(acb_per_share1);
      // acb_per_share = temp->get_acb_per_share();
      temp->set_cgl(temp->get_amount() - (temp->get_shares() * acb_per_share1)); 
    }
   
    // if (temp->get_next() != nullptr)
    // {
    //   temp->get_next()->set_acb(temp->get_acb());
    //   temp->get_next()->set_share_balance(temp->get_share_balance());
    //   temp->get_next()->set_acb_per_share(temp->get_acb_per_share());
    // }
    temp = temp->get_next();
  }

//    double tempAcb = 0;
//    double tempSbalance = 0;
//    double tempApshare = 0;

//    while (temp != nullptr) {
//        if (temp->get_trans_type()) {
//            temp->set_acb(tempAcb + temp->get_amount());
//            tempAcb = temp->get_acb();
//            temp->set_share_balance(tempSbalance + temp->get_shares());
//            tempSbalance = temp->get_share_balance();
//            temp->set_acb_per_share(tempAcb / tempSbalance);
//            tempApshare = temp->get_acb_per_share();
//        } else {
//            temp->set_acb(tempAcb - (temp->get_shares() * tempApshare));
//            tempAcb = temp->get_acb();
//            temp->set_share_balance(tempSbalance - temp->get_shares());
//            tempSbalance = temp->get_share_balance();
//            temp->set_cgl(temp->get_amount() - (temp->get_shares() * tempApshare));
//        }
// //        if (temp->get_next() != nullptr) {
// //            temp->get_next()->set_acb(temp->get_acb());
// //            temp->get_next()->set_share_balance(temp->get_share_balance());
// //            temp->get_next()->set_acb_per_share(temp->get_acb_per_share());
// //        }
//        temp = temp->get_next();
//    }

    
}



// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year)
{
  //tcg stands for total capital gain
  double tcg = 0;
  Transaction* temp = p_head;
  while (temp != nullptr)
  {
    if (temp->get_year() == year)
    {
      tcg += temp->get_cgl();
    }
    temp = temp->get_next();
  }

  delete temp;
  // temp = nullptr;
  return tcg;
}

// print() Print the transaction history.
// TASK 9
//
void History::print()
{
  std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
  Transaction* fuckyou_transaction = p_head;
  while (fuckyou_transaction != nullptr)
  {
    fuckyou_transaction -> print();
    fuckyou_transaction = fuckyou_transaction -> get_next();
  }

  std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
