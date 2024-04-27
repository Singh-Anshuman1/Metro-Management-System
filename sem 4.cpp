#include <iostream>

#include <unordered_map>

#include <queue>

#include <string>

#include <limits>

#include <vector>

#include <algorithm>

using namespace std;

const int MAX_N = 100; // maximum amount
const int MAX_M = 10;  // maximum number of coin denominations

int no_of_denominations;
int c[MAX_M];             // array of coin denominations
int dp[MAX_N + 1][MAX_M]; // dynamic programming table
int inf = 1e9;            // a large value to represent infinity

// Station struct to store station information
struct Station
{
    bool visited;
    int distance;
    string prev;
    unordered_map<string, int> neighbors;
};

// Function to search for a station and return its index
int searchStation(string stationName, vector<string> &stations)
{
    for (int i = 0; i < stations.size(); i++)
    {
        if (stations[i] == stationName)
        {
            return i;
        }
    }
    return -1;
}

// Dijkstra's algorithm to find shortest path
int dijkstra(string start, string end, unordered_map<string, Station> &stations)
{
    // Set all station distances to infinity
    for (auto &s : stations)
    {
        s.second.distance = numeric_limits<int>::max();
        s.second.visited = false;
    }

    // Set starting station distance to 0
    stations[start].distance = 0;

    // Create a priority queue to store unvisited stations

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    // Add starting station to priority queue

    pq.push(make_pair(0, start));

    while (!pq.empty())
    {
        // Get station with shortest distance from priority queue

        pair<int, string> current = pq.top();
        pq.pop();

        // If current station is the end station, we have found the shortest path
        if (current.second == end)
        {
            break;
        }

        // Mark current station as visited
        stations[current.second].visited = true;

        // Go through all neighbors of current station

        for (auto &neighbor : stations[current.second].neighbors)
        {
            string neighborName = neighbor.first;
            int neighborDistance = neighbor.second;

            // If neighbor station has already been visited, skip it
            if (stations[neighborName].visited)
            {
                continue;
            }

            // Calculate new distance to neighbor station

            int distance = stations[current.second].distance + neighborDistance;

            // If new distance is shorter than previous distance, update it

            if (distance < stations[neighborName].distance)
            {
                stations[neighborName].distance = distance;
                stations[neighborName].prev = current.second;

                // Add neighbor station to priority queue

                pq.push(make_pair(distance, neighborName));
            }
        }
    }

    // Print shortest path

    if (stations[end].prev.empty())
    {
        cout << "No path found" << endl;
    }
    else
    {
        string station = end;
        while (station != start)
        {
            cout << station << " <-- ";
            station = stations[station].prev;
        }
        cout << start << endl;
        cout << endl;
        cout << "TOTAL DISTANCE: " << stations[end].distance << " KMs" << endl;
        cout << "TOTAL TIME: " << stations[end].distance * 3 << " Min" << endl;
    }

    return stations[end].distance;
}

// Backtracking to find the possible paths
void backtracking(string start, string end, unordered_map<string, Station> &stations, vector<string> &path, vector<vector<string>> &allPaths)
{
    path.push_back(start);
    if (start == end)
    {
        allPaths.push_back(path);
    }
    else
    {
        Station &station = stations[start];
        for (auto &neighbor : station.neighbors)
        {
            string neighborName = neighbor.first;
            if (find(path.begin(), path.end(), neighborName) == path.end())
            {
                backtracking(neighborName, end, stations, path, allPaths);
            }
        }
    }
    path.pop_back();
}

// Dynamic Programming to Solve the Coinage Problem
void coinage(int target_value, int no_of_denominations)
{
    // initialize dynamic programming table
    for (int i = 1; i <= target_value; i++)
    {
        for (int j = 0; j < no_of_denominations; j++)
        {
            if (i == c[j])
            {
                dp[i][j] = 1;
            }
            else
            {
                dp[i][j] = inf;
            }
        }
    }

    // fill dynamic programming table
    for (int i = 1; i <= target_value; i++)
    {
        for (int j = 0; j < no_of_denominations; j++)
        {
            for (int k = 0; k <= j; k++)
            {
                if (i - c[k] >= 0)
                {
                    dp[i][j] = min(dp[i][j], dp[i - c[k]][k] + 1);
                }
            }
        }
    }

    // print denominations required to make change for n
    vector<int> d(no_of_denominations, 0); // vector to store denominations
    int i = target_value, j = no_of_denominations - 1;
    while (i > 0 && j >= 0)
    {
        if (j > 0 && dp[i][j - 1] <= dp[i][j])
        {
            // exclude coin denomination c[j]
            j--;
        }
        else
        {
            // include coin denomination c[j]
            d[j]++;
            i -= c[j];
        }
    }

    // print results
    if (i > 0)
    {
        cout << "No solution found." << endl;
    }
    else
    {
        for (int j = 0; j < no_of_denominations; j++)
        {
            cout << "Rs." << c[j] << " : " << d[j] << " units" << endl;
        }
    }
}

int main()
{
    // Create vector of station names
    vector<string> stations = {"Rithala", "Rohini West", "Pitampura", "Netaji Subhash Place", "Kirti Nagar", "Rajouri Garden", "Janakpuri West", "Dwarka Mor", "Dwarka"};

    // Create map of stations
    unordered_map<string, Station> stationMap;

    // Add stations to map
    for (auto &station : stations)
    {
        stationMap[station] = Station();
    }

    // Add neighbors and distances for each station
    stationMap["Rithala"].neighbors = {{"Rohini West", 2}, {"Dwarka", 5}};
    stationMap["Rohini West"].neighbors = {{"Rithala", 2}, {"Pitampura", 3}, {"Rajouri Garden", 7}, {"Kirti Nagar", 3}};
    stationMap["Pitampura"].neighbors = {{"Rohini West", 3}, {"Netaji Subhash Place", 2}, {"Dwarka Mor", 1}};
    stationMap["Netaji Subhash Place"].neighbors = {{"Pitampura", 2}, {"Kirti Nagar", 3}};
    stationMap["Kirti Nagar"].neighbors = {{"Netaji Subhash Place", 3}, {"Rajouri Garden", 2}, {"Janakpuri West", 4}, {"Rohini West", 2}};
    stationMap["Rajouri Garden"].neighbors = {{"Kirti Nagar", 2}, {"Janakpuri West", 4}, {"Rohini West", 7}};
    stationMap["Janakpuri West"].neighbors = {{"Rajouri Garden", 4}, {"Dwarka Mor", 4}, {"Kirti Nagar", 1}};
    stationMap["Dwarka Mor"].neighbors = {{"Janakpuri West", 4}, {"Dwarka", 3}, {"Pitampura", 1}};
    stationMap["Dwarka"].neighbors = {{"Dwarka Mor", 3}, {"Rithala", 5}};

    // Get source and destination stations from user
    string source, destination;
    cout << endl;
    cout << endl;
    cout << "ENTER THE SOURCE STATION: ";
    getline(cin, source);
    cout << endl;
    cout << "ENTER THE DESTINATION STATION: ";
    getline(cin, destination);
    cout << endl;
    cout << endl;
    cout << endl;

    // Search for source and destination stations in station vector
    int sourceIndex = searchStation(source, stations);
    int destinationIndex = searchStation(destination, stations);

    // If either station is not found, print error message and return
    if (sourceIndex == -1 || destinationIndex == -1)
    {
        cout << "Error: Station not found" << endl;
        return 0;
    }

    // PRINTING ALL POSSIBLE PATHS USING BACKTRACKING
    cout << "**********************************";
    cout << endl;
    cout << "DEAR PASSENGER! ALL POSSIBLE ROUTES BETWEEN THE SOURCE AND DESTINATION STATIONS ARE AS FOLLOWS:";
    cout << endl;
    cout << "**********************************";
    cout << endl;

    vector<string> path;
    vector<vector<string>> allPaths;
    backtracking(stations[sourceIndex], stations[destinationIndex], stationMap, path, allPaths);

    if (allPaths.empty())
    {
        cout << "No path found\n";
    }
    else
    {
        int count = 1;
        for (vector<string> p : allPaths)
        {
            cout << count << ".) ";
            count++;
            for (string station : p)
            {
                if (station == destination)
                    cout << station << endl;
                else
                    cout << station << " -> ";
            }
            cout << endl;
        }
    }

    cout << endl;
    cout << endl;

    // Find shortest path using Dijkstra's algorithm
    cout << "********************";
    cout << endl;
    cout << "DEAR PASSENGER! YOUR SHORTEST PATH WILL BE THE FOLLOWING:";
    cout << endl;
    cout << "********************";
    cout << endl;
    int distance = dijkstra(stations[sourceIndex], stations[destinationIndex], stationMap);
    int cost = 1.7 * distance + 10;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // Calculate the Total Cost for the journey
    cout << "****************";
    cout << endl;
    cout << "DEAR PASSENGER! YOUR COST FOR THE JOURNEY:";
    cout << endl;
    cout << "****************";
    cout << endl;
    cout << "COST: Rs." << cost;
    cout << endl;
    cout << "----";
    cout << endl;
    cout << endl;

label:
    cout << "For payment by card, PRESS: 1 (10% effective discount)";
    cout << endl;
    cout << "For payment by cash, PRESS: 2";
    cout << endl;
    cout << endl;
    int input;
    cout << "ENTER YOUR CHOICE: ";
    cout << endl;
    cout << "-----------------";
    cin >> input;
    cout << endl;
    cout << endl;

    switch (input)
    {
    case 1:
        cout << "DISCOUNTED COST = Rs." << 0.9 * cost;
        cout << endl;
        cout << "---------------";
        cout << endl;
        cout << "SAVINGS = Rs." << 0.1 * cost;
        cout << endl;
        cout << "-------";
        break;
    case 2:
        // Give the no. of denominations required.
        cout << "Enter The No. Of Denominations You Have: ";
        cin >> no_of_denominations;
        cout << endl;
        cout << "Enter Your Denominations: ";
        for (int i = 0; i < no_of_denominations; i++)
        {
            cin >> c[i];
        }
        cout << endl;
        cout << endl;
        cout << "YOU HAVE TO PAY: ";
        cout << endl;
        cout << "---------------";
        cout << endl;
        coinage(cost, no_of_denominations);

        break;
    default:
        goto label;
    }

    cout << endl;
    cout << endl;
    cout << endl;

    cout << "*******************";
    cout << endl;
    cout << "THANK YOU FOR YOUR JOURNEY WITH US. SEE YOU AGAIN SOON!";
    cout << "*******************";

    return 0;
}
