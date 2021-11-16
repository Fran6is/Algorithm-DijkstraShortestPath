#include <iostream>

#include <vector>
#include <algorithm>
#include <queue>
#include <list>

struct Edge
{
	char EndPoint_Tag;
	int  EndPoint_Index = 0;
	int  Weight;
};

struct Vertex
{
	int  Index = -1;
	char Tag = '1';
	std::vector<Edge> Edges;
};

void AddVertex(std::vector< Vertex >& Vertices, char Tag)
{
	Vertices.emplace_back(Vertex{ (int)Vertices.size(), Tag });

}

struct DColumn
{
	char Vertex;

	//Shortest distance from path start vertex
	int  ShortestDistance = INT32_MAX;

	char PreviousVertex = '0';

	bool bVisited = false;
};


std::ostream& operator<<(std::ostream& cout, const Vertex& Rhs);
std::ostream& operator<<(std::ostream& cout, const std::vector< Vertex >& Rhs);

bool MakeConnections(std::vector<Vertex>& GraphVertices, char Vertex1, char Vertex2, int Weight);

int GetVertexIndex( const std::vector<Vertex>& GraphVertices, char VertexTag);


//Get a vertex from a list of Graph vertices
const Vertex& GetVertex(const std::vector<Vertex>& GraphVertices, char VertexTag)
{
	auto FoundVertex = std::find_if(GraphVertices.begin(), GraphVertices.end(), [=](const Vertex& vertex) {

		return vertex.Tag == VertexTag;

		});

	return *FoundVertex;
}

//Get a column from a list of "Dijstra's table" ( for lack of a better term )
DColumn& GetTableColumn(std::vector<DColumn>& Table, char VertexTag)
{
	auto FoundColumn = std::find_if(Table.begin(), Table.end(), [=](const DColumn& Column) {

		return Column.Vertex == VertexTag;

		});

	return *FoundColumn;
}


void TraverseGraph(const std::vector< Vertex >& Vertices, char Start);



std::ostream& operator<<(std::ostream& cout, const DColumn& Column);
std::ostream& operator<<(std::ostream& cout, const std::vector<DColumn>& Table);

int main()
{
	std::vector< Vertex > Vertices;

	AddVertex(Vertices, 'a');
	AddVertex(Vertices, 'b');
	AddVertex(Vertices, 'c');
	AddVertex(Vertices, 'd');
	AddVertex(Vertices, 'e');

	MakeConnections(Vertices, 'a', 'b', 6);
	MakeConnections(Vertices, 'a', 'd', 1);

	MakeConnections(Vertices, 'b', 'a', 6);
	MakeConnections(Vertices, 'b', 'e', 2);
	MakeConnections(Vertices, 'b', 'd', 2);
	MakeConnections(Vertices, 'b', 'c', 5);

	MakeConnections(Vertices, 'd', 'a', 1);
	MakeConnections(Vertices, 'd', 'b', 2);
	MakeConnections(Vertices, 'd', 'e', 1);

	MakeConnections(Vertices, 'e', 'd', 1);
	MakeConnections(Vertices, 'e', 'b', 2);
	MakeConnections(Vertices, 'e', 'c', 5);

	MakeConnections(Vertices, 'c', 'b', 5);
	MakeConnections(Vertices, 'c', 'e', 5);

	std::cout << "\nGRAPH::Log\n";
	std::cout << "\t[vertex] : [edge](weight), ... \n\n";
	std::cout << Vertices;


	

	char StartVertex = 'c';
	

	//Create a table

	std::vector<DColumn> Table;
	Table.reserve(Vertices.size());
	Table.emplace_back(DColumn{ StartVertex, 0});

	

	for (auto&  Vert: Vertices)
	{
		if (Vert.Tag != StartVertex)
		{
			Table.emplace_back( DColumn{ Vert.Tag } );
		}
	}

	//
	std::cout << "\nTABLE::Before:\n";
	std::cout << "Start vertex = " << StartVertex << "\n";
	std::cout << Table;
	std::cout << "\n";

	while( true )
	{
		//find an unvisted vertex in the table with the smallest known distance from the start vertex
		int UnvisitedTableVertexWsmallestDist_Index = -1;

		bool bFirstTime = true;
		for (size_t i = 0; i < Table.size(); i++)
		{
			if ( !Table[i].bVisited && bFirstTime )
			{

				UnvisitedTableVertexWsmallestDist_Index = i;
				bFirstTime = false;
			}

			if ( !Table[i].bVisited )
			{
				UnvisitedTableVertexWsmallestDist_Index = Table[i].ShortestDistance < Table[UnvisitedTableVertexWsmallestDist_Index].ShortestDistance 
														? i : UnvisitedTableVertexWsmallestDist_Index;
			}
		}

		if (UnvisitedTableVertexWsmallestDist_Index == -1)
		{
			std::cout << "All table columns have been visited...Exiting loop \n";
			break;
		}


		size_t CurrentTableVertex_I = UnvisitedTableVertexWsmallestDist_Index;

		for ( auto& CurrentEdge : GetVertex(Vertices, Table[CurrentTableVertex_I].Vertex ).Edges )
		{
			int DistanceToEdge = CurrentEdge.Weight + Table[CurrentTableVertex_I].ShortestDistance;

			DColumn& TableColumn = GetTableColumn(Table, CurrentEdge.EndPoint_Tag);

			if (  DistanceToEdge < TableColumn.ShortestDistance )
			{
				TableColumn.ShortestDistance = DistanceToEdge;
				TableColumn.PreviousVertex   = Table[CurrentTableVertex_I].Vertex;
			}

		}

		Table[CurrentTableVertex_I].bVisited = true;
	}
	

	std::cout << "\nTABLE::After: \n";
	std::cout << Table;
	std::cout << "\n";

	
	return 0;
}



std::ostream& operator<<(std::ostream& cout, const std::vector<DColumn>& Table )
{
	std::cout << "\n\t" << " [ Vertex ]" << " | " << " [ Shortest Path from Start vertex ] " << " | " << " [Previous Vertex] " << "\n\n";

	for (auto& Column : Table)
	{
		/*
		cout << "\t" << "    " << Column.Vertex << "    " << " | " 
			 << "               "  << Column.ShortestDistance << "               " << " | " 
			 << "        " << Column.PreviousVertex << "        " << "\n";
		*/

		if (Table.begin()->Vertex == Column.Vertex)
			std::cout << " -> ";

		std::cout << Column;
	}

	return cout;
}

std::ostream& operator<<(std::ostream& cout, const DColumn& Column)
{
	
	cout << "\t" << Column.Vertex << " | " << Column.ShortestDistance << " | " << Column.PreviousVertex << "\n";

	return cout;
}

void TraverseGraph(const std::vector< Vertex >& Vertices, char Start)
{
	bool* Visited = new bool[Vertices.size()];

	for (size_t i = 0; i < Vertices.size(); i++)
		Visited[i] = false;

	std::queue<int> Queue;

	int StartIndex = GetVertexIndex(Vertices, Start);

	Visited[StartIndex] = true;
	Queue.push( StartIndex );

	std::vector<Edge>::const_iterator It_Edge;

	while (!Queue.empty())
	{
		int CurrentVertex = Queue.front();

		std::cout << Vertices[CurrentVertex].Tag << " ";

		Queue.pop();

		for (It_Edge = Vertices[CurrentVertex].Edges.begin(); It_Edge != Vertices[CurrentVertex].Edges.end(); It_Edge++)
		{
			if (!Visited[It_Edge->EndPoint_Index]) //see if the index has been visited
			{
				Visited[It_Edge->EndPoint_Index] = true;
				Queue.push(It_Edge->EndPoint_Index);
			}
		}
	}




	delete[] Visited;
}

int GetVertexIndex(const std::vector<Vertex>& GraphVertices, char VertexTag)
{
	auto FoundVertex = std::find_if(GraphVertices.begin(), GraphVertices.end(), [=](Vertex vertex) {

		return vertex.Tag == VertexTag;

		});

	return FoundVertex != GraphVertices.end() ? FoundVertex->Index : -1;
}


bool MakeConnections(std::vector<Vertex>& GraphVertices, char Vertex1, char Vertex2, int Weight)
{
	bool bBothArePresent = false;

	int Vertex1_ID = -1, Vertex2_ID = -1;

	//Vertex1 search
	auto FoundVertex = std::find_if(GraphVertices.begin(), GraphVertices.end(), [=](Vertex vertex) {

		return vertex.Tag == Vertex1;

		});

	if (bBothArePresent = FoundVertex != GraphVertices.end()) Vertex1_ID = FoundVertex->Index;

	//vertex2 search
	FoundVertex = std::find_if(GraphVertices.begin(), GraphVertices.end(), [=](Vertex vertex) {

		return vertex.Tag == Vertex2;

		});

	if (FoundVertex != GraphVertices.end()) Vertex2_ID = FoundVertex->Index;

	bBothArePresent = bBothArePresent && (FoundVertex != GraphVertices.end());


	if (!bBothArePresent)
	{
		std::cerr << "No Connection made between '" << Vertex1 << "' and '" << Vertex2 << "' because one or both vertices doesn't exist\n";
		return false;
	}

	GraphVertices.at(Vertex1_ID).Edges.emplace_back( Edge{ Vertex2, Vertex2_ID, Weight } );

	//std::cout << "Connection made between '" << Vertex1 << "' and '" << Vertex2 << "'\n";

	return true;
}



std::ostream& operator<<(std::ostream& cout, const std::vector< Vertex >& Rhs)
{

	for (auto& Vert : Rhs)
	{
		cout << Vert << "\n";
	}

	return cout;
}


std::ostream& operator<<(std::ostream& cout, const Vertex& Rhs)
{
	std::cout << "\t" << Rhs.Tag << " : ";

	int i = 0;
	for (auto& Edge : Rhs.Edges)
	{
		cout << Edge.EndPoint_Tag << "(" << Edge.Weight << ")" << (Rhs.Edges.size() - 1 == i ? " " : ", ");

		i++;
	}
	std::cout << "\n";

	return cout;
}
