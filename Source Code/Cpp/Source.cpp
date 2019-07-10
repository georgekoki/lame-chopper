/*
	Lame cutter v1 by George Kokiadis
	https://github.com/georgekoki
	georgekoki1999@gmail.com

	Chop videos into works of art

	This file contains all the source code of the app.
	Required external libs: OpenCV 4.1.0
	Please consult the readme
*/

/* Last edit: 1:22AM 10 Jul 2019 */
#define FOREGROUND_WHITE 15
#define FOREGROUND_YELLOW 14
#define FOREGROUND_CYAN 11
#define FOREGROUND_RED 12
#define FOREGROUND_DARKBLUE 3
#define FOREGROUND_GREEN 10

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <Windows.h>

using namespace cv;
using namespace std;

/*
	@brief
		Allocate a 2D array using C arrays
	@params
		r: rows
		c: columns
	@return
		Successfully allocated 2D Array
*/
int** MM_2DArrayAllocator(int r, int c) {
	int** arr = (int**)malloc(r * sizeof(int*));
	for (int i = 0; i < r; i++)
		arr[i] = (int*)malloc(c * sizeof(int));

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			arr[i][j] = 0;
	return arr;
}

/*
	@brief
		Gets a Column from a given frame
	@params
		frame: frame Matrix that we are processing
	@return
		A 2D Array with dimensions (frame height) x 3 that contains all the BRG values of each pixel
		of the column
*/
int** getColumn(Mat frame) {
	uint8_t* image_data = frame.data;				// Data we will be using from the frame
	int width = frame.cols;							// Width of video
	int height = frame.rows;						// Height of video
	int _stride = frame.step;						// If we see data as a 2D array, this is its width
													// with its height beeing the height of the video

	int** final_column = MM_2DArrayAllocator(height, 3);					// Allocate the column we will return

	for (int video_line = 0; video_line < height; video_line++) {			// For every line of pixels in the frame
		for (int video_column = 0; video_column < width; video_column++) {	// Get every column of that line to make a pixel
			for (int pixel = 0; pixel < 3; pixel++) {						// And every color value of that pixel
				int image_data_index = video_column * 3 + pixel + video_line * _stride;	// Calculate the 1D array index
				// Add all the color values of every pixel in the line so we can divide by the column size and find the median
				final_column[video_line][pixel] = final_column[video_line][pixel] + image_data[image_data_index];
			}
		}
	}
	// Find the median of all values
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < 3; j++) {
			final_column[i][j] = final_column[i][j] / width;
		}
	}

	return final_column;
}

/*
	@brief
		Initializes the program with all the values we need
	@param
		step: Pointer to the step variable in main
		output_filename: Pointer to output filename variable in main
	@return
		Valid VideoCapture to read data from
*/
VideoCapture initialize(int* step, std::string * output_filename) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "	**********************************\n";
	cout << "	*";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
	cout << "         Lame Chopper           ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "*\n";
	cout << "	*";
	SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
	cout << " Code by George 'Koki' Kokiadis ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "*\n";
	cout << "	*";
	SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
	cout << " https://github.com/georgekoki  ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "*\n";
	cout << "	**********************************\n\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_CYAN);
	cout << "	Welcome to lame chopper. I really apologise for this terrifying command prompt.\n";
	cout << "	I will try my best to make your experience as painless as possible.\n";
	cout << "	If you have no idea what this program does, please consult the readme!\n\n";
	cout << "	Let's get started. Please provide me with the name of the file you would like to use.\n";
	cout << "	(Note: the file has to be in the same folder with the executable file";
	cout << ", and you need to \n	include the extension of the file (.mp4, .avi))\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
	cout << "File = ";
	String filename;
	VideoCapture video, temp;
	cin >> filename;
	while (!video.open(filename)) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "/!\\ File not found or filename was wrong.\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
		cout << "File = ";
		cin >> filename;
	}
	temp = video;
	Mat image;
	temp.read(image);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Found file " << filename << " successfully. Some info about your video:\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
	cout << "	- Dimensions (wxh) = " << image.rows << "x" << image.cols << "\n";
	cout << "	- Frames per second = " << temp.get(CAP_PROP_FPS) << "\n";
	cout << "	- Total ammount of frames = " << temp.get(CAP_PROP_FRAME_COUNT) << "\n\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Given this information, how many frames would you like me to skip per column?\n";
	char ans = 'n';
	do {
		do {
			SetConsoleTextAttribute(hConsole, FOREGROUND_DARKBLUE);
			cout << "(Careful: If you input something other than a number, i will freak out.)\n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
			cout << "Step = ";
			cin >> *step;
		} while (*step < 0 || *step > temp.get(CAP_PROP_FRAME_COUNT));
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		cout << ">> You have given a step of " << *step << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
		cout << "	This means you will need to export " << floor(temp.get(CAP_PROP_FRAME_COUNT) / *step) << " columns in total\n";
		cout << "	Resulting in an image of size (wxh) " << image.rows << "x" << floor(temp.get(CAP_PROP_FRAME_COUNT) / *step) << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
		cout << "Are you happy with that? (y/n): ";
		cin >> ans;
	} while (ans != 'y' && ans != 'Y');
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Please pick the name of the file you want to generate\n";
	do {
		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
		cout << "Output file name = ";
		cin >> *output_filename;
		cout << "Are you happy with the filename " << *output_filename << "? (y/n): ";
		cin >> ans;
	} while (ans != 'y' && ans != 'Y');
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Starting...\n";
	return video;
}

/*
	@brief
		Prints a progress bar
	@param
		progress: % to print
*/
void printProgressBar(float progress, bool flag) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int barWidth = 70;
	SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
	std::cout << "[";
	SetConsoleTextAttribute(hConsole, FOREGROUND_CYAN);
	int pos = barWidth * progress;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "#";
		else if (i == pos) std::cout << "#";
		else std::cout << " ";
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
	std::cout << "] ";

	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
	std::cout << int(progress * 100.0);
	if (!flag)
		std::cout << "%\r";
	else
		std::cout << "%\n";
}

int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int step;
	string output_filename;
	VideoCapture cap = initialize(&step, &output_filename);
	int video_frame_count = cap.get(CAP_PROP_FRAME_COUNT);

	int total_frames = video_frame_count / step + 1;
	Mat image;
	vector<int**> image_vector;
	cap.read(image);

	int image_rows = image.rows;
	int count = 0;
	cout << ">> Processing your video...\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
	while (cap.read(image)) {
		image_vector.push_back(getColumn(image));
		for (int i = 0; i < step; i++) {
			cap.grab();
		}
		count++;
		printProgressBar((float)count / (float)total_frames, false);
	}
	printProgressBar(1, true);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Processing done, rendering final image...\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);

	Mat line_matrix(image_rows, image_vector.size(), CV_8UC3);
	for (int row = 0; row < image_rows; row++) {
		for (int col = 0; col < image_vector.size(); col++) {
			for (int pixel = 0; pixel < 3; pixel++) {
				line_matrix.data[row * image_vector.size() * 3 + col * 3 + pixel] = image_vector.at(col)[row][pixel];
			}
		}
	}

	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	imwrite(output_filename + ".png", line_matrix, compression_params);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << ">> Rendering done! You can find your image in the folder this executable was in.\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
	cout << "Press a key and enter to exit...";
	char c;
	cin >> c;
	return 0;

}