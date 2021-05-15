#include "tom.h"
#include "jerry.h"


Tom::Tom(int initialRow, int initialColumn, int d[20][20])
{

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            data[i][j] = d[i][j];
    row = initialRow;
    column = initialColumn;
    // Set Image
    QPixmap image("tomm.png");
    image = image.scaledToWidth(35);
    image = image.scaledToHeight(35);
    setPixmap(image);
    // Set Position
    setPos(50 + 35 * column, 50 + 35 * row);
    step = 1;

    for (int i = 0; i < COUNT; i++)
        {
        for (int j = 0; j < COUNT; j++)
                adjMatrix[i][j] = 0;
        }
    for (int i = 0; i <= 19; i++)
        {   for (int j = 0; j <= 19; j++)
               {
              int vertex = data[i][j];

             if (data[i][j] >=0 && !inHome(vertex))
 {

          if (data[i][j + 1] >= 0 && !inHome(data[i][j+1]))
          {
              adjMatrix[vertex][data[i][j+1]] = 1;
          }
            if(data[i-1][j]>=0 && !inHome(data[i - 1][j]))

            {
              adjMatrix[vertex][data[i-1][j]] = 1;

          }
            if(data[i][j-1]>=0 && !inHome(data[i][j - 1]))
          {
              adjMatrix[vertex][data[i][j-1]] = 1;
          }
            if(data[i+1][j]>=0 && !inHome(data[i + 1][j]))
          {

           adjMatrix[vertex][data[i+1][j]] = 1;
          }

          else
          {
              adjMatrix[vertex][data[i][j]] = 0;
          }
                 }
      }

    }

}

void Tom::ReturnToOriginalPos()
{
    row = 18;
    column = 10;
    setPos(50 + 35 * column, 50 + 35 * row);
}

QVector<QVector<int> > Tom::Dijkstra(int Graph[COUNT][COUNT], int startVertex)
{
       int temp[COUNT][COUNT];
    for (int i = 0; i < COUNT; i++)
       {
           for (int j = 0; j < COUNT; j++)
           {
               if (Graph[i][j] == 0)
                   temp[i][j] = INFINITE;
               else
                   temp[i][j] = Graph[i][j];
           }
       }
       bool visited[COUNT];
       int previous[COUNT];
       float cost[COUNT];
       // 1st Row:
       for (int i = 0; i < COUNT; i++)
       {
           if (i == startVertex)
           {
               previous[i] = -1;
               cost[i] = 0;
               visited[i] = true;
           }
           else
           {
               previous[i] = startVertex;
               cost[i] = temp[startVertex][i];
               visited[i] = false;
           }
       }
       // All Rows:
       int count = 1;
       while (count < COUNT)
       {
           // Determine which vertex to visit.
           int minimum = INFINITE, visitedVertex;
           for (int i = 0; i < COUNT; i++)
           {
               if (visited[i] == false && cost[i] < minimum)
               {
                   minimum = cost[i];
                   visitedVertex = i;
               }
           }
           // Visit the vertex.
           visited[visitedVertex] = true;

           // Check whether there are shorter paths.
           for (int i = 0; i < COUNT; i++)
           {
               if (visited[i] == false)
               {
                   if ((cost[visitedVertex] + temp[visitedVertex][i]) < cost[i])
                   {
                       previous[i] = visitedVertex;
                       cost[i] = (cost[visitedVertex] + temp[visitedVertex][i]);
                   }
               }
           }
           count++;
       }
       // Extract the paths.
       QVector<QVector<int>> paths;
       paths.resize(COUNT);
       int j;
       for (int i = 0; i < COUNT; i++)
       {
           paths[i].push_back(i);
           if (i != startVertex)
           {
               j = i;
               do
               {
                   j = previous[j];
                   paths[i].insert(paths[i].begin(), j);
               } while (j != startVertex);
           }
       }

       return paths;

}


void Tom::advance()//advancing the position, responsible for moving
{
    qreal speed = 5;
    RandMotion();


setPos(mapToParent(0, -speed));
}


void Tom::UpdateMotion()
{
    if(moving){
    auto path = Dijkstra(adjMatrix, data[row][column]);
    QString str = "";
    for(auto p : path[jerryVertex])
        str += QString::number(p) + " ";

     if(path[jerryVertex].size() > 1)
     {
         int nextNode = path[jerryVertex][1];
         int leftNode = data[row][column - 1];
         int rightNode = data[row][column + 1];
         int upNode = data[row - 1][column];
         int downNode = data[row + 1][column];

         if(nextNode  == leftNode)
             column--;
         else if (nextNode ==rightNode)
             column++;
         else if( nextNode == upNode)
             row--;
         else if(nextNode == downNode)
             row++;
     }
     setPos(50 + 35 * column, 50 + 35 * row);
    }

}

void Tom::SetJerryVertex(int v)
{

    jerryVertex = v;
}

void Tom::stoppMoving()
{
    moving=false;
}


void Tom::RandMotion()
{
    srand(time(NULL));
    int x = qrand()% 4 + 1;
    //for ( int i=0; i < 13; i++)
    //{
        if (x==1 && data[row - 1][column] != -10)// && data[row - 1][column] != home[i])
        {
            int i;
            for (i=0; i < 13; i++)
                if (data[row - 1][column] == home[i])
                    break;
            if (i == 13)
                row--;
        }
        else if (x==2 && data[row + 1][column] != -10)// && data[row + 1][column] != home[i])
        {
            int i;
            for (i=0; i < 13; i++)
                if (data[row + 1][column] == home[i])
                    break;
            if (i == 13)
            row++;
        }
        else if (x==3 && data[row][column + 1] != -10)// && data[row][column + 1] != home[i])
        {
            int i;
            for (i=0; i < 13; i++)
                if (data[row][column + 1] == home[i])
                    break;
            if (i == 13)
            column++;
        }
        else if (x==4 && data[row][column - 1] != -10)// && data[row][column - 1] != home[i])
        {
            int i;
            for (i=0; i < 13; i++)
                if (data[row][column - 1] == home[i])
                    break;
            if (i == 13)
            column--;
        }
    //}
    setPos(50 + 35 * column, 50 + 35 * row);
}





