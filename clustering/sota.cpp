// Class automatically generated by Dev-C++ New Class wizard
#include <cstring>
#include "sota.h" // class's header file

using std::string;

const float sota::VTDefault; // variability threshold
const float sota::RTDefault; // resource threshold
const float sota::ETDefault; // relative error threshold
const int sota::CNDefault; // maximum number of clycles
const float sota::A1Default;
const float sota::A2Default;
const float sota::A3Default;  

// class constructor
sota::sota(string datasetName, string fileResultName, t_similarities sim = euclidean,
           float variability = VTDefault, float resource = RTDefault, 
           float error = ETDefault, int nCycle = CNDefault, 
           float act1 = A1Default, float act2 = A2Default, float act3 = A3Default)
{
	dataset = datasetName;
	fileResult = fileResultName;
    similarity = sim;
    VT = variability; // variability threshold
    RT = resource; // resource threshold
    ET = error; // relative error threshold
    CN = nCycle; // maximum number of clycles
    A1 = act1;
    A2 = act2;
    A3 = act3;  
    
    //std::cout << "VT:" << VT << " RT:" << RT << " ET:" << ET << std::endl;
    //std::cout << "CN:" << CN << " A1:" << A1 << " A2:" << A2 << std::endl;
    // definir tambem o VT (escolher um valor)

}

sota::sota(string datasetName, string fileResultName, t_similarities sim = euclidean)
{
	dataset = datasetName;
	fileResult = fileResultName;
    similarity = sim;
    VT = VTDefault; // variability threshold
    RT = RTDefault; // resource threshold
    ET = ETDefault; // relative error threshold
    CN = CNDefault; // maximum number of clycles
    A1 = A1Default;
    A2 = A2Default;
    A3 = A3Default;  
}


sota::sota()
{
	dataset = "";
	fileResult = "";
    similarity = euclidean;
    VT = VTDefault; // variability threshold
    RT = RTDefault; // resource threshold
    ET = ETDefault; // relative error threshold
    CN = CNDefault; // maximum number of clycles
    A1 = A1Default;
    A2 = A2Default;
    A3 = A3Default;  
}

sota::sota(std::string datasetName, std::string fileResultName, t_similarities sim, std::string parameters)
{
    dataset = datasetName;
	fileResult = fileResultName;
    similarity = sim;
    VT = VTDefault; // variability threshold
    RT = RTDefault; // resource threshold
    ET = ETDefault; // relative error threshold
    CN = CNDefault; // maximum number of clycles
    A1 = A1Default;
    A2 = A2Default;
    A3 = A3Default;  
    
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");
    tokenizer tokens(parameters, sep);
    
    tokenizer::iterator ti = tokens.begin();
    while (ti != tokens.end())
    {
        if (*ti == "-cn")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            CN = atoi((*ti).c_str());
            if ((CN == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "CN: " << CN << std::endl;
        }
        else if (*ti == "-sim")  
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            if (*ti == "euclidean")
            {
                similarity = euclidean;
            } 
            else if (*ti == "pearson")
            {
                similarity = pearson;
            } 
            else
            {                
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "sim: " << similarity << std::endl;
        }
        else if (*ti == "-vt")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            VT = atof((*ti).c_str());
            if ((VT == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "vt: " << VT << std::endl;
        }
        else if (*ti == "-rt")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            RT = atof((*ti).c_str());
            if ((RT == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "rt: " << RT << std::endl;
        }
        else if (*ti == "-et")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            ET = atof((*ti).c_str());
            if ((ET == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "et: " << ET << std::endl;
        }
        else if (*ti == "-a")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A1 = atof((*ti).c_str());
            if ((A1 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A2 = atof((*ti).c_str());
            if ((A2 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A3 = atof((*ti).c_str());
            if ((A3 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            //std::cout << "a: " << A1 << " " << A2 << " " << A3 << std::endl;
        }
        else std::cout << "Invalid parameter: " << *ti << std::endl;
        ti++;
    }  
}


sota::sota(std::string parameters)
{
    dataset = "";
	fileResult = "";
    similarity = euclidean;
    VT = VTDefault; // variability threshold
    RT = RTDefault; // resource threshold
    ET = ETDefault; // relative error threshold
    CN = CNDefault; // maximum number of clycles
    A1 = A1Default;
    A2 = A2Default;
    A3 = A3Default;  
    
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");
    tokenizer tokens(parameters, sep);
    
    tokenizer::iterator ti = tokens.begin();
    while (ti != tokens.end())
    {
        if (*ti == "-cn")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            CN = atoi((*ti).c_str());
            if ((CN == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else if (*ti == "-sim")  
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            if (*ti == "euclidean")
            {
                similarity = euclidean;
            } 
            else if (*ti == "pearson")
            {
                similarity = pearson;
            } 
            else
            {                
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else if (*ti == "-vt")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            VT = atof((*ti).c_str());
            if ((VT == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else if (*ti == "-rt")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            RT = atof((*ti).c_str());
            if ((RT == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else if (*ti == "-et")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            ET = atof((*ti).c_str());
            if ((ET == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else if (*ti == "-a")
        {
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A1 = atof((*ti).c_str());
            if ((A1 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A2 = atof((*ti).c_str());
            if ((A2 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
            ti++;
            if (ti == tokens.end()) {std::cout << "Error in the parameters." << std::endl; break;}
            A3 = atof((*ti).c_str());
            if ((A3 == 0) && (*ti != "0"))
            {
                std::cout << "Invalid parameter value: " << *ti << std::endl;
            }
        }
        else std::cout << "Invalid parameter: " << *ti << std::endl;
        ti++;
    }  
}


// class destructor
//sota::~sota()
//{
//	// insert your code here
//}

sota::sota(const sota &a)
{
    dataset = a.dataset;
    fileResult = a.fileResult;
    similarity = a.similarity;
    VT = a.VT; 
    RT = a.RT; 
    ET = a.ET; 
    CN = a.CN; 
    A1 = a.A1;
    A2 = a.A2;
    A3 = a.A3;  
    p = a.p;
} 

sota & sota::operator = (const sota &a)
{
    dataset = a.dataset;
    fileResult = a.fileResult;
    similarity = a.similarity;
    VT = a.VT; 
    RT = a.RT; 
    ET = a.ET; 
    CN = a.CN; 
    A1 = a.A1;
    A2 = a.A2;
    A3 = a.A3;  
    p = a.p;
} 

cpartition sota::run()
{ 
    string command, sim;

    // similarities available in the software sota
    // "euclidean", "correlation", "square"
    // "offset", "spearman", "jackknife"
    switch(similarity) 
    {
        case euclidean:   sim = "euclidean"; break;
        case pearson:   sim = "correlation"; break;
    }
    
    if (runningSystem == mywindows)
        command = "sota " + dataset + " " + fileResult + " " +  sim;
    else
        command = "sota " + dataset + " " + fileResult + " " +  sim;

    if (VT != VTDefault)    
    {
        command += " -v ";
        command += boost::lexical_cast<string>(VT);
    }  
  
    if (RT != RTDefault)    
    {
        command += " -r ";
        command += boost::lexical_cast<string>(RT);
    }

    if (ET != ETDefault)    
    {
        command += " -e ";
        command += boost::lexical_cast<string>(ET);
    }

    if (CN != CNDefault)    
    {
        command += " -max ";
        command += boost::lexical_cast<string>(CN);
    }
    
    if ((A1 != A1Default) || (A2 != A2Default) || (A3 != A3Default))
    {
        command += " -a ";
        command += boost::lexical_cast<string>(A1);
        command += " ";
        command += boost::lexical_cast<string>(A2);
        command += " ";
        command += boost::lexical_cast<string>(A3);
    }  

    //std::cout << command << std::endl;

    
    system(command.c_str());

    // grava os resultados no formato .clu e carrega o agrupamento
    command = "perl convertSOTA-CLU.pl " + fileResult + ".sot " + fileResult + ".clu";
    system(command.c_str());
    
    string fileClu = fileResult + ".clu";
    p.loadPartition(fileClu);

    return p;

}

void sota::changeDataset(std::string datasetName)
{
     dataset = datasetName;
}

void sota::changeResultFile(std::string fileResultName)
{
     fileResult = fileResultName;
}

void sota::changeSimilarity(t_similarities sim)
{
     similarity = sim;
}

void sota::changeVT(float auxVT) // variability threshold (0)
{
     VT = auxVT;
}

void sota::changeRT(float auxRT) // resource threshold (0)
{
     RT = auxRT;
}

void sota::changeET(float auxET) // relative error threshold (0.0001)
{
     ET = auxET;
}
 
void sota::changeA1(float auxA1)
{
     A1 = auxA1;
}

void sota::changeA2(float auxA2)
{
     A2 = auxA2;
}

void sota::changeA3(float auxA3) // actualization factors: winning node, mother node, 
// sister node (0, 0, 0)
{
     A3 = auxA3;
}

void sota::changeCN(int auxCN) // maximum number of clycles
{
     CN = auxCN;
}


cpartition sota::getPartition()
{
    return p;
}

sota::t_cells sota::getCells()
{    
     
//    t_cells clusterCenters;

    fs::ifstream file2,file3;
    char line[maxFileLine];
    string line2, identifier, s;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");
    std::map<std::string, std::vector<double>, cmp> pat;
 
    // load the correspondence between nodes and clusters in sota results
    std::map<int, int > clustersNodes;
    int node, cluster;    
    string fileCorrespondence = fileResult + ".clu.correspondence";
    //string fileCorrespondence = fileAlgorithmPartition.string() + ".clu.correspondence";
    //std::cout << fileCorrespondence << std::endl;
    file2.open(fileCorrespondence);
    if (!file2.good()) 
    {
        std::cout << "\nError in the file '" << fileCorrespondence << "'\n";
        getchar();
    }    
    while (!file2.eof())
    {
        file2 >> cluster >> node;
        //std::cout << cluster << std::endl;
        //getchar();
        clustersNodes[node] = cluster;
    }
    file2.close();
    
    //getchar();

    std::map<int, std::vector<double> > clustersCenters;

    // read the cells of the file .cod generated with sotarray 6.0
    string fileSotaCells = fileResult + ".cod";
    //string fileSotaCells = fileAlgorithmPartition.string() + ".cod";
    //char nodeWord[10], sign[10];
    file3.open(fileSotaCells);
    if (!file3.good()) 
    {
        std::cout << "\nError in the file '" << fileSotaCells << "'\n";
        getchar();
    }
    
    file3.getline(line, maxFileLine);
    file3.getline(line, maxFileLine);
    
    while (!file3.eof())
    {
        file3.getline(line, maxFileLine);
        
        sscanf(line, "%*s%d", &node);
//        strcmp(sign, "->")
        if (strstr(line, "->"))
        {
            file3.ignore(maxFileLine, '\n');
            //std::cout << "tem sign" << std::endl;
//            std::cout << nodeWord << std::endl;
            //std::cout << node << std::endl;
        }
        else
        {     
            //std::cout << "entrou no else" << std::endl;     
            //std::cout << node << std::endl;
            file3.getline(line, maxFileLine);
            line2 = line;
            tokenizer tokens(line2, sep);
            //tokens.assign(line2, sep);
            for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
            {
                s = *tok_iter; 
                //std::cout << s << std::endl;
                clustersCenters[clustersNodes[node]].insert(clustersCenters[clustersNodes[node]].end(), atof(s.c_str()));
                //pat[identifier].insert(pat[identifier].end(), atof(s.c_str())); 
            //std::cout << s.c_str() << std::endl;
            }
        }
   }
    file3.close();

//        std::map<int, std::vector<double> >::iterator patIterator;        
//        for (patIterator = clustersCenters.begin(); 
//        patIterator != clustersCenters.end(); 
//        patIterator++)    
//        {
//            std::cout << "First " << (*patIterator).first << std::endl;
//            for (std::vector<double>::iterator v = (*patIterator).second.begin(); v != (*patIterator).second.end(); ++v)
//            { 
//                std::cout << "Second " << *v << std::endl; 
//            }
//        }
//getchar();

    return clustersCenters;
}

