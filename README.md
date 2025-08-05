# DSA-Project-3

This program will ask you for requirements and will build an optimized squad to use in FIFA based on two different algorithms. 

The following explains how the inputs are handled:
- First the program will ask if you have male_players.csv loaded to you IDE locally.
    - If you do have it downloaded locally, type --->  y
    - If you dont have it downloaded locally, but it is downloaded to your computer, type ---> n
          - If you typed 'n', you must now input the absolute path to male_players.csv on your computer. Do not put quotations around it.
- Then, the program will ask for a formation for your squad. The input must include a dash in between each number. Ex: 4-3-3
- Next, the program will ask for a budget between 0 - 2,000,000. Input your budget without commas
- Next, The program will ask for a mininum rating for your squad. Input an integer between 0 and 99.
- Next, the program will ask for a mininum chemistry requirement. Input an integer between 0 and 33.
- Then the program will ask if you have any nations, league, or clubs requirements. For each one you will be asked to input y/n for your choice.
  - If you say yes to adding any of these requirements, you are then required to type the correct name of the nation,league, or club. (You can check the csv file for correct spelling if you need).
  - If you have a list for any of these requirements, seperate each entry with a comma.
  - After you've done that, you now need to input the amount of players you want from each nation, league, or club. If your input is in a list, seperate each number with a comma. (EX: 3 nations. Brazil, England, France. 1,1,1)
  - Finally you are prompted with which algorithm you'd like to use to build the squad. Type 1,2,3 depending on the algorithm you'd like the program to implement. 



If you're not someone familiar with this game here are some sample test cases:
Test Case 1:
4-2-3-1
2000000
80
0
y
England
1
y
Premier League
1
y
Manchester City
1
3

Test Case 2:
4-3-3
2000000
88
0
n
n
n
3

Test Case 3:
3-4-3
2000000
80
10
y
Portugal, England
1, 1
y
Liga Portugal, Premier League
1, 1
y
Sporting CP, Manchester United
1, 1
3

Test Case 4: 
4-4-2
1000000
70
0
y
Argentina
1
y
Serie A
1
n
3

Test Case 5:
4-3-3
2000000
0
33
n
n
n
3
