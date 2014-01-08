%SOLUTION TO PROJECT 3 -- FALL 2013
%AUTHOR: S. M. ZOLTEK
clear
%Clear the current figure -- if one exists
clf

load tridata.dat
num_stations=length(tridata)
pause

%Calcuate the matrix of distances between pairs of stations
dist_list=zeros(num_stations,num_stations)
for i=1:num_stations
    for j=1:num_stations
        dist_list(i,j)=sqrt((tridata(i,1)-tridata(j,1))^2+(tridata(i,2)-tridata(j,2))^2);
    end
end

dist_list
pause

m=num_stations/3
tri_list=zeros(m*m*m,3)
pause


state1_list=1:m
state2_list=m+1:2*m
state3_list=2*m+1:3*m
pause

%Create the matrix that holds the solution triangles and corresponding lengths
answerlist=zeros(m,4)
pause


l=0;

%Display stations in each state
for count=1:m
    state1_list
    state2_list
    state3_list
    pause

    %Now generate all triangles
    mm=0;
    tri_list=zeros(length(state1_list)^3,3);
    for i=1:m-l
        for j=1:m-l
            for k=1:m-l
                mm=mm+1;
                tri_list(mm,1)=state1_list(i);
                tri_list(mm,2)=state2_list(j);
                tri_list(mm,3)=state3_list(k);

            end
        end
    end

    %Display the list of triangles
    tri_list
    pause

    %Now calculate the perimeters of each triangle of tri_list
    %displaying the results as a column
    [lm,ln]=size(tri_list)
    perimeter_list=zeros(lm,1);

    for i= 1:lm
        l1=dist_list(tri_list(i,1),tri_list(i,2));
        l2=dist_list(tri_list(i,2),tri_list(i,3)); 
        l3=dist_list(tri_list(i,3),tri_list(i,1));
        perimeter_list(i)=l1+l2+l3;
    end
    perimeter_list
    pause

    %Append the calculated column at the right of tri_list
    [tri_list,perimeter_list]
    pause


    %Now find the minimum perimeter
    [per_min, I_min]=min(perimeter_list);

    %Find the stations in the triangle with minimum perimeter
    station_in_state1=tri_list(I_min,1);
    station_in_state2=tri_list(I_min,2);
    station_in_state3=tri_list(I_min,3);

    %Place the triangle at its permiter in answerlist
    answerlist(count,1)=station_in_state1;
    answerlist(count,2)=station_in_state2;
    answerlist(count,3)=station_in_state3;
    answerlist(count,4)=per_min;
    answerlist
    pause

    %Now eliminate these powerstations from their lists
    if l < m-1
        for i=1:length(state1_list)
            if state1_list(i)==station_in_state1
                state1_list(i)=0;
            end
            if state2_list(i)==station_in_state2
                state2_list(i)=0;
            end
            if state3_list(i)==station_in_state3
                state3_list(i)=0;
            end
        end

    %Now sort the lists and eliminate the 0 from them
        state1_list=sort(state1_list)
        state2_list=sort(state2_list)
        state3_list=sort(state3_list)

        state1_list=state1_list(1,2:length(state1_list))
        state2_list=state2_list(1,2:length(state2_list))
        state3_list=state3_list(1,2:length(state3_list))
    end
    pause

    %now increase l to l+1 and plot the result

    xlist=[tridata(station_in_state1,1)
    tridata(station_in_state2,1)
    tridata(station_in_state3,1)
    tridata(station_in_state1,1)]
    ylist=[tridata(station_in_state1,2)
    tridata(station_in_state2,2)
    tridata(station_in_state3,2)
    tridata(station_in_state1,2)]
    hold on
    plot(xlist,ylist)
    plot(tridata(station_in_state1,1),tridata(station_in_state1,2),'*')
    plot(tridata(station_in_state2,1),tridata(station_in_state2,2),'+')
    plot(tridata(station_in_state3,1),tridata(station_in_state3,2),'o')
    hold off

    l=l+1;
    pause

end


