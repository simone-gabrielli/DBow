#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// DBow
#include "dbow/DUtils.h"
#include "dbow/DBow.h"

using namespace DBow;
using namespace DUtils;
using namespace std;

// Number of training images
const int Nimages = 4;

// SuperPoint descriptors are 256-dimensional
const int descriptor_dim = 256;

void loadFeatures(vector<vector<float>> &features);
void testVocCreation(const vector<vector<float>> &features);
void testDatabase(const vector<vector<float>> &features);

void wait()
{
    cout << endl << "Press enter to continue" << endl;
    getchar();
}

int main()
{
    vector<vector<float>> features;
    loadFeatures(features);

    testVocCreation(features);

    wait();

    testDatabase(features);

    wait();

    return 0;
}

// Function to load SuperPoint features
void loadFeatures(vector<vector<float>> &features)
{
    features.clear();
    features.reserve(Nimages);

    cout << "Extracting SuperPoint features..." << endl;

    for (int i = 1; i <= Nimages; i++)
    {
        stringstream ss;
        ss << "image" << i << ".png";

        cv::Mat image = cv::imread(ss.str(), cv::IMREAD_GRAYSCALE);
        if (image.empty())
        {
            cerr << "Error loading image: " << ss.str() << endl;
            continue;
        }

        // SuperPoint feature extraction
        vector<cv::KeyPoint> keypoints;
        vector<float> descriptors;

        // Simulating SuperPoint extraction (Replace with actual inference)
        for (int j = 0; j < 500; j++) // Assume 500 keypoints per image
        {
            keypoints.push_back(cv::KeyPoint(rand() % image.cols, rand() % image.rows, 1));
            vector<float> desc(descriptor_dim, static_cast<float>(rand()) / RAND_MAX); // Random descriptors
            descriptors.insert(descriptors.end(), desc.begin(), desc.end());
        }

        features.push_back(descriptors);
    }
}

void testVocCreation(const vector<vector<float>> &features)
{
    // Branching factor and depth levels
    const int k = 10;
    const int L = 3;

    HVocParams params(k, L, descriptor_dim);
    HVocabulary voc(params);

    cout << "Creating a " << k << "^" << L << " vocabulary..." << endl;
    voc.Create(features);
    cout << "... done!" << endl;

    cout << "Stopping some words..." << endl;
    voc.StopWords(0.01f);

    cout << "Vocabulary information: " << endl;
    cout << endl
         << voc.RetrieveInfo().toString() << endl;

    // Matching images against themselves
    cout << "Matching images (0 low, 1 high): " << endl;
    BowVector v1, v2;
    for (int i = 0; i < Nimages; i++)
    {
        voc.Transform(features[i], v1);
        for (int j = i + 1; j < Nimages; j++)
        {
            voc.Transform(features[j], v2);

            double score = voc.Score(v1, v2);
            cout << "Image " << i + 1 << " vs Image " << j + 1 << ": " << score << endl;
        }
    }

    cout << endl
         << "Saving vocabulary..." << endl;
    voc.Save("superpoint_vocabulary.txt", false);
    cout << "Done" << endl;
}

void testDatabase(const vector<vector<float>> &features)
{
    cout << "Creating a small database..." << endl;

    HVocabulary *voc = new HVocabulary("superpoint_vocabulary.txt");
    Database db(*voc);
    delete voc; // DB maintains its own vocabulary instance

    for (int i = 0; i < Nimages; i++)
    {
        db.AddEntry(features[i]);
    }

    cout << "... done!" << endl;

    cout << "Database information: " << endl;
    cout << endl
         << db.RetrieveInfo().toString() << endl;

    cout << "Querying the database: " << endl;

    QueryResults ret;
    for (int i = 0; i < Nimages; i++)
    {
        db.Query(ret, features[i], 2);

        cout << "Searching for Image " << i + 1 << ". Best match: "
             << ret[1].Id + 1 << ", score: " << ret[1].Score << endl;
    }
}
