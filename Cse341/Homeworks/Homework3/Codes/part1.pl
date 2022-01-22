:- dynamic student/3.
:- dynamic course/6.
:- dynamic room/5.


%    ID   Capacity    Hours            Special Equip.         Handicapped
room(z23  , 50 ,     [10,11]     ,  [projector,smartBoard]  ,   hasAccess).
room(z08  , 30 ,     [8,9,13,14] ,  [smartBoard]            ,   noAccess).
room(z10  , 40 ,     [12,13]     ,  [none]                  ,   hasAccess).
room(z11  , 40 ,     [15,16,17]  ,  [projector,smartBoard]  ,   noAccess).

%       Room ID  Hours     Course          
occupancy(z23, [10,11] ,   cse321).
occupancy(z08, [8,9]   ,   cse341).
occupancy(z08, [13,14] ,   cse343).
occupancy(z10, [12,13] ,   cse331).

%     Course ID   Instructor        Capacity   Course Hours      Rooms  SpecialNeed
course(cse341, 'Yakup Genc'         , 4,        [8,9]         ,  [z08], smartBoard).
course(cse321, 'Didem Gozupek'      , 5,        [10,11]       ,  [z23], projector).
course(cse331, 'Alp Arslan Bayrakci', 3,        [12,13]       ,  [z10], none).
course(cse343, 'Habil Kalkan'       , 2,        [13,14]       ,  [z08], smartBoard).

%           Instructor Name         Courses      Preference
instructor('Yakup Genc'          , [cse341]   , smartBoard).
instructor('Didem Gozupek'       , [cse321]   , smartBoard).
instructor('Alp Arslan Bayrakci' , [cse331]   , none).
instructor('Habil Kalkan'        , [cse343]   , smartBoard).

%    Student ID       Courses          Handicapped or not
student('A' , [cse341, cse321, cse331] , handicapped   ).
student('C' , [cse331, cse321]         , notHandicapped).
student('D' , [cse343]                 , handicapped   ).
student('H' , [cse331]                 , handicapped   ).
student('L' , [cse341, cse321, cse343] , notHandicapped).



% Query 1
has_conflict(Course1, Course2):-
    course(Course1,_,_,Hours1,_,_),
    course(Course2,_,_,Hours2,_,_),
    contains(Hours1, Hours2),
    format('Comparing hours ~w and ~w',[Hours1, Hours2]), nl,
    format('There is a conflict').

has_conflict(Course1, Course2):-
    course(Course1,_,_,Hours1,_,_),
    course(Course2,_,_,Hours2,_,_),
    not(contains(Hours1, Hours2)),
    format('Comparing hours ~w and ~w',[Hours1, Hours2]),nl,
    format('There is no conflict').

% Query 2
which_room(Course):-
    course(Course,Instructor,_,CourseHours,_,SpecialNeed),
    instructor(Instructor,_,InstructorPreference),
    room(Room,_,RoomHours,Equipments,_),
    not(contains(CourseHours, RoomHours)),
    member(InstructorPreference, Equipments),
    member(SpecialNeed, Equipments),
    format('Room                              : ~w',[Room]), nl,
    format('Course Instructor                 : ~w',[Instructor]), nl,
    format('Instructor preference             : ~w',[InstructorPreference]), nl,
    format('Course special need               : ~w',[SpecialNeed]), nl,
    format('Room ~w has these equipments     : ~w', [Room, Equipments]), nl,
    format('Room has courses at these hours   : ~w', [RoomHours]), nl,
    format('Course ~w held at these hours : ~w', [Course, CourseHours]), nl,
    format('-> Equipments are enough and there are no time conflicts.'), nl,
    format('-> Course ~w can held at room ~w', [Course, Room]).

% Query 3
which_room_to_where():-
    course(Course,_,_,_,_,_),
    which_room(Course).

% Query 4
can_enroll(Student, Course):-
    student(Student,_, Handicapped),
    course(Course,_,_,_,Rooms,_),
    room(Room,_,_,_,HasAccess),
    member(Room, Rooms),
    Handicapped = handicapped,
    HasAccess = hasAccess,
    format('Student        : ~w',[Student]), nl,
    format('Condition      : ~w',[Handicapped]), nl,
    format('Course         : ~w',[Course]), nl,
    format('Course held at : ~w',[Room]), nl,
    format('Room Status    : ~w',[HasAccess]), nl,
    format('-> Student ~w can enroll to course ~w at room ~w',[Student, Course, Room]), nl.

can_enroll(Student, Course):-
    student(Student,_, Handicapped),
    course(Course,_,_,_,Rooms,_),
    room(Room,_,_,_,_),
    member(Room, Rooms),
    Handicapped = notHandicapped,
    format('Student        : ~w',[Student]), nl,
    format('Condition      : ~w',[Handicapped]), nl,
    format('Course         : ~w',[Course]), nl,
    format('Course held at : ~w',[Room]), nl,
    format('-> Student ~w can enroll to course ~w at room ~w',[Student, Course, Room]), nl.

% Query 5
which_classes_can_be_assigned(Student):-
    course(Course,_,_,_,_,_),
    can_enroll(Student, Course).

add_student(Name, Courses, Handicapped):-
    assert(student(Name , [Courses], Handicapped)).

add_course(Name, Instructor, Capacity, Hours, Rooms, SpecialNeed):-
    assert(course(Name, Instructor, Capacity, Hours, Rooms, SpecialNeed)).

add_room(Name, Capacity, Hours, Equipment, Access):-
    assert(room(Name, Capacity, Hours, Equipment, Access)).

contains([Head|_], Hours2):-
    member(Head, Hours2).
contains([_|Tail], Hours2):-
    contains(Tail, Hours2).
