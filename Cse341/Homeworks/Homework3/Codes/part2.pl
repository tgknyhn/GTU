%% Knowledge-base
% canakkale
flight(canakkale, erzincan, 6).
% erzincan
flight(erzincan, canakkale, 6).
flight(erzincan, antalya, 3).
% antalya
flight(antalya, erzincan, 3).
flight(antalya, izmir, 2).
flight(antalya, diyarbakir, 4).
% izmir
flight(izmir, antalya, 2).
flight(izmir, istanbul, 2).
flight(izmir, ankara, 6).
% istanbul
flight(istanbul, izmir, 2).
flight(istanbul, ankara, 1).
flight(istanbul, rize, 4).
% ankara
flight(ankara, izmir, 6).
flight(ankara, istanbul, 1).
flight(ankara, rize, 5).
flight(ankara, van, 4).
flight(ankara, diyarbakir, 8).
% rize
flight(rize, istanbul, 4).
flight(rize, ankara, 5).
% diyarbakir
flight(diyarbakir, ankara, 8).
flight(diyarbakir, antalya, 4).
% van
flight(van, ankara, 4).
flight(van, gaziantep, 3).
% gaziantep
flight(gaziantep, van, 3).

%% Rules
route(X,Y,C):-
    flight(X,Y,C).

route(X,Y,C):-
    visit(X,Y,0,[X],C).

visit(X,Y,C,Visited,WantedCost):-
    flight(X,Y,Cost),
    not(member(Y, Visited)),
    FinalCost is Cost + C,
    FinalCost = WantedCost, 
    reverse(Visited, Path),
    append(Path, [Y], FinalPath),
    last_element(X2, Visited),
    nl,
    format('Start  : ~w',[X2]), nl,
    format('Finish : ~w',[Y]), nl,
    format('Path   : ~w',[FinalPath]), nl.


visit(X,Y,C,Visited,WantedCost):-
    flight(X,Z,Cost),
    Y \== Z,
    not(member(Z, Visited)),
    TotalCost is C + Cost,
    visit(Z,Y, TotalCost, [Z|Visited],WantedCost).

last_element(X,[X]).

last_element(X,[_|Z]):- 
    last_element(X,Z).
