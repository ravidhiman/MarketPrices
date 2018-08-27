#include<iostream>
#include<string>
#include<set>
#include<conio.h>
using namespace std;

class Order
{
	char *OrderId;
	int Price;
	int BidOrOffer;
public:
	Order(const char *order_id,int price,int  bid_offer);
	int getprice() const;
	int getBidOrOffer() const;
	char* getOrderId() const;
	friend ostream& operator<<(ostream & out,const Order& odr);
};

Order::Order(const char *order_id = NULL,int price = 0,int  bid_offer = 0)
{
	int len = strlen(order_id);
	OrderId = new char[len+1];
	strcpy(OrderId,order_id);
	Price = price;
	BidOrOffer = bid_offer;
}
int Order::getprice() const
{
	return Price;
}
int Order::getBidOrOffer() const
{
	return BidOrOffer;
}
char* Order::getOrderId() const
{
	return OrderId;
}

ostream& operator<<(ostream & out,const Order& odr)
{
	out<<odr.OrderId<<" "<<odr.Price<<" "<<odr.BidOrOffer;
	return out;
}

//comparator
struct cmp1
{
public:
	bool operator()(const Order &lhs,const Order &rhs)
	{
		if(lhs.getprice()<rhs.getprice())
			return true;
		else
			return false;	
	}
};

struct cmp2
{
public:
	bool operator()(const Order &lhs,const Order &rhs)
	{
		if(lhs.getprice()>rhs.getprice())
			return true;
		else
			return false;	
	}
};

class OrderBook
{
	char *ProductId;
	set<Order,cmp2> myBidOrders;
	set<Order,cmp1> myOfferorders;
public:
	OrderBook(const char *product_id);
	void insert_Bid(const Order &odr);
	void insert_offer(const Order &odr);
	char * getProductId() const;
	int delete_order(char *OrderId);
	friend ostream& operator<<(ostream & out,const OrderBook& odr);
};

OrderBook::OrderBook(const char *product_id)
{
	int len = strlen(product_id);
	ProductId = new char[len+1];
	strcpy(ProductId,product_id);
}
void OrderBook::insert_Bid(const Order &odr)
{
	myBidOrders.insert(odr);
}
void OrderBook::insert_offer(const Order &odr)
{
	myOfferorders.insert(odr);
}
char * OrderBook::getProductId() const
{
	return ProductId;
}
int OrderBook::delete_order(char *OrderId)
{
	set<Order,cmp2>::const_iterator it1;
	set<Order,cmp1>::const_iterator it2;
	it1 = myBidOrders.begin();
	while(it1!=myBidOrders.end())
	{
		if(!strcmp(it1->getOrderId(),OrderId))
			myBidOrders.erase(it1++);
		else
			it1++;
	}
	it2 = myOfferorders.begin();
	while(it2!=myOfferorders.end())
	{
		if(!strcmp(it2->getOrderId(),OrderId))
			myOfferorders.erase(it2++);
		else
			it2++;
	}
	return 1;
}

ostream& operator<<(ostream & out,const OrderBook& odr)
{
	out<<odr.ProductId<<":"<<endl;
	set<Order,cmp2>::const_iterator it1;
	set<Order,cmp1>::const_iterator it2;
	out<<"Buy:"<<endl;
	for(it1 = odr.myBidOrders.begin();it1 != odr.myBidOrders.end();it1++)
		out<<*it1<<" "<<endl;
	out<<"Sell:"<<endl;
	for(it2  = odr.myOfferorders.begin();it2 != odr.myOfferorders.end();it2++)
		out<<*it2<<" "<<endl;
	return out;
}
struct cmp3
{
	public:
	bool operator()(const OrderBook &lhs,const OrderBook &rhs)
	{
		return (strcmp(lhs.getProductId(),rhs.getProductId())<0);
	}
};
class MarketPrices {
public:
	virtual int OnOrderAdd(char *productId, char *OrderId, int BidOrOffer, int Price) = 0;
	virtual int OnOrderDel(char *productId, char *OrderId) = 0;
	virtual int Print() = 0;
};

class FinalOrder:public MarketPrices
{
	set<OrderBook,cmp3> myOrderbooks;
public:
	int OnOrderAdd(char *productId, char *OrderId, int BidOrOffer, int Price);
	int OnOrderDel(char *productId, char *OrderId);
	int Print();
};

int FinalOrder::OnOrderAdd(char *productId, char *OrderId, int BidOrOffer, int Price)
{
	Order objorder(OrderId,Price,BidOrOffer);
	set<OrderBook,cmp3>::iterator itr;
	itr = myOrderbooks.find(productId);
	if(itr!=myOrderbooks.end())
	{
		if(BidOrOffer == 1)
			itr->insert_Bid(objorder);
		else
			itr->insert_offer(objorder);
	}
	else
	{
		OrderBook objOrderBook(productId);
		if(BidOrOffer == 1)
			objOrderBook.insert_Bid(objorder);
		else
			objOrderBook.insert_offer(objorder);
		myOrderbooks.insert(objOrderBook);
	}
	return 1;
}
int FinalOrder::OnOrderDel(char *productId, char *OrderId)
{
	set<OrderBook,cmp3>::iterator itr;
	itr = myOrderbooks.find(productId);
	if(itr!=myOrderbooks.end())
	{
		itr->delete_order(OrderId);
	}
	return 1;
}
int FinalOrder::Print()
{
	set<OrderBook,cmp3>::iterator itr;
	for(itr = myOrderbooks.begin();itr!=myOrderbooks.end();itr++)
	{
		cout<<*itr;
	}
	return 1;
}


int main()
{
	FinalOrder myFinalOrder;
	myFinalOrder.OnOrderAdd("product1", "order1", 1, 100);
	myFinalOrder.OnOrderAdd("product1", "order2", 1, 101);
	myFinalOrder.OnOrderAdd("product2", "order3", 1, 99);
	myFinalOrder.OnOrderAdd("product2", "order4", 2, 100);
	cout<<"Printing the orders : \n\n";
	myFinalOrder.Print();
	cout<<"\nDeleting the order2 of product1\n\n"; 
	myFinalOrder.OnOrderDel("product1","order2");
	cout<<"Printing the orders after deleting above order: \n\n";
	myFinalOrder.Print();
	getch();
	return 0;
}