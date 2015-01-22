#include <set>
#include <cassert>
#include <sstream>
#include <iostream>

#include "test.h"

namespace Sibelia
{
	void DnaStringTest(size_t n, std::ostream & log)
	{
		DnaString str0(32);
		for (size_t i = 0; i < n; i++)
		{
			size_t idx = rand() % str0.GetSize();			
			char newChar = DnaString::LITERAL[rand() % DnaString::LITERAL.size()];
			log << "Setting char, str(" << idx << ") = " << newChar << std::endl;
			str0.SetChar(idx, newChar);
			log << "Got char " << str0.GetChar(idx) << std::endl;
			assert(str0.GetChar(idx) == newChar);
		}


		DnaString str1;
		std::string str2;
		for (size_t i = 0; i < n; i++)
		{
			log << i << '\t';
			if (str1.GetSize() < 32 && rand() % 2)
			{
				char ch = DnaString::LITERAL[rand() % DnaString::LITERAL.size()];
				if (rand() % 2)
				{
					log << "Appending back";
					str1.AppendBack(ch);
					str2.push_back(ch);
				}
				else
				{
					log << "Appending front";
					str1.AppendFront(ch);
					str2.insert(str2.begin(), ch);
				}
			}
			else if (str1.GetSize() > 0)
			{
				if (rand() % 2)
				{
					log << "Popping back";
					str1.PopBack();
					str2.erase(str2.end() - 1);
				}
				else
				{
					log << "Popping front";
					str1.PopFront();
					str2.erase(str2.begin());
				}
			}

			log << "\tSize=" << str1.GetSize() << std::endl;				
			std::string str1b = str1.ToString();
			log << str1b << std::endl << str2 << std::endl;
			assert(str1b == str2);
		}
	}

	void VertexEnumeratorTest(const std::vector<std::string> & fileName, size_t vertexLength, size_t filterSize, std::ostream & log)
	{
		std::set<std::string> edges;
		size_t edgeLength = vertexLength + 1;
		VertexEnumerator vid(fileName, vertexLength, filterSize);

		for (const std::string & nowFileName : fileName)
		{
			bool start = true;
			for (StreamFastaParser parser(nowFileName); parser.ReadRecord(); start = true)
			{
				char ch;
				std::string edge;
				for (size_t j = 0; j < edgeLength && parser.GetChar(ch); j++)
				{
					edge.push_back(ch);
				}

				if (edge.size() >= edgeLength)
				{
					while (true)
					{
						edges.insert(edge);
						if (parser.GetChar(ch))
						{
							edge.push_back(ch);
							edge.erase(edge.begin());
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		std::set<std::string> bif;
		for (const std::string & nowFileName : fileName)
		{
			for (StreamFastaParser parser(nowFileName); parser.ReadRecord();)
			{
				char ch;
				std::string vertex;
				for (size_t j = 0; j < vertexLength && parser.GetChar(ch); j++)
				{
					vertex.push_back(ch);
				}

				if (vertex.size() >= vertexLength)
				{
					while (true)
					{
						size_t inCount = 0;
						size_t outCount = 0;
						for (char ch : DnaString::LITERAL)
						{
							std::string inEdge = ch + vertex;
							std::string outEdge = vertex + ch;
							inCount += edges.count(inEdge);
							outCount += edges.count(outEdge);
						}

						if (inCount != 1 || outCount != 1)
						{
							DnaString check;
							for (size_t i = 0; i < vertex.size(); i++)
							{
								check.AppendBack(vertex[i]);
							}

							assert(vid.GetId(check) != VertexEnumerator::INVALID_VERTEX);
							bif.insert(vertex);
						}

						if (parser.GetChar(ch))
						{
							vertex.push_back(ch);
							vertex.erase(vertex.begin());
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		std::cout << "TP = " << bif.size() << std::endl;
		std::cout << "FP = " << vid.GetVerticesCount() - bif.size() << std::endl;
	}

	bool Runtests()
	{
		std::stringstream ss;
		std::vector<std::string> fileName;
	//	fileName.push_back("g1.fasta");
	//	fileName.push_back("g2.fasta");
	//	VertexEnumeratorTest(fileName, 9, (1 << 28) + 1, ss);
		DnaStringTest(10000, std::cerr);
		
		return true;
	}
}