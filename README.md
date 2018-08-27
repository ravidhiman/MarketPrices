# MarketPrices-
Write a container class in which bids and offers (also called orders) related to multiple products are stored.

The MarketPrices class is made up of different orderbooks. An orderbook is a set of orders
associated to the same productId. Each orderbook can have bid and offer orders which are
sorted based on the Price of the order.

Bids have to be ordered based on the Price in descending order while Offers have to be ordered
based on the Price in ascending order.
It is possible to print the depth on the standard output using the print method.

Examples:
OnOrderAdd(“product1”, “order1”, 1, 100)
OnOrderAdd(“product1”, “order2”, 1, 101)
OnOrderAdd(“product2”, “order3”, 1, 99)
OnOrderAdd(“product2”, “order4”, 2, 100)
Print has the following result:
product 1:
Buy:
order2 101
order1 100
Sell:
product 2:
Buy:
order3 99
Sell:
order4 100
