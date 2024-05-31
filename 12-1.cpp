// 날짜: 2024/05/13
// 작성자 : 2001197 서현석
/*1번과제
#include <opencv2/opencv.hpp> // OpenCV 라이브러리 포함
#include <iostream> // 표준 입출력 라이브러리 헤더 파일을 포함
using namespace cv; // OpenCV 네임스페이스 사용
using namespace std; // 표준 네임스페이스(std)를 사용

int main() {
	Mat src = imread("labelex.png", IMREAD_GRAYSCALE);// 이미지 파일을 그레이스케일로 읽어옴
	Mat bin, labels, stats, mid;
	double x, y;
	threshold(src, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);// 이진화 수행
	int cnt = connectedComponentsWithStats(bin, labels, stats, mid);// 연결 요소 레이블링을 통해 객체의 통계 정보 계산
	double max_x = mid.at<double>(1, 0), max_y = mid.at<double>(1, 1), min_x = mid.at<double>(1, 0), min_y = mid.at<double>(1, 1);
	int area, max_area = 0, max = 1, min_area = 0, min = 1;
	for (int i = 1; i < stats.rows; i++) {// 모든 객체에 대해 반복
		x = mid.at<double>(i, 0);
		y = mid.at<double>(i, 1);
		area = stats.at<int>(i, 4);
		if (area > max_area) {max_area = area;max = i;max_x = x;max_y = y;}// 최대 면적 확인
		if (area < min_area) {min_area = area;min = i;min_x = x;min_y = y;}// 최소 면적 확인
	}
	cvtColor(src, src, COLOR_GRAY2BGR);// 컬러 이미지로 변환
	rectangle(src, Rect(stats.at<int>(min, 0), stats.at<int>(min, 1), stats.at<int>(min, 2), stats.at<int>(min, 3)), Scalar(255, 0, 0));// 최소 면적를 파란색으로 표시
	rectangle(src, Rect(stats.at<int>(max, 0), stats.at<int>(max, 1), stats.at<int>(max, 2), stats.at<int>(max, 3)), Scalar(0, 0, 255));// 최대 면적를 빨간색으로 표시
	cout << "면적이 최대인 객체의 레이블:" << max << endl;
	cout << "무게중심:(x,y): " << max_x << "\t" << max_y << endl;
	cout << "면적이 최소인 객체의 레이블:" << min << endl;
	cout << "무게중심:(x,y): " << min_x << "\t" << min_y << endl;
	imshow("src", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
/*2번 과제
#include <opencv2/opencv.hpp> // OpenCV 라이브러리 포함
#include <iostream> // 표준 입출력 라이브러리 헤더 파일을 포함
using namespace cv; // OpenCV 네임스페이스 사용
using namespace std; // 표준 네임스페이스(std)를 사용
int main() {
	Mat src = imread("polygon.bmp");
	Mat gray, bin, labels, stats, mid;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 150, 255, THRESH_BINARY_INV);
	int cnt = connectedComponentsWithStats(bin, labels, stats, mid);
	cout << "갯수 " << cnt << endl;
	cout << "label x y width height area color[B,G,R]" << endl;
	for (int i = 1; i < cnt; i++) {
		double x = mid.at<double>(i, 0);
		double y = mid.at<double>(i, 1);
		rectangle(src, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 255, 255));
		putText(src, to_string(i), Point(x, y),	FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
		cout << i << " " << stats.at<int>(i, 0) << " " << stats.at<int>(i, 1) << " " 
			<< stats.at<int>(i, 2) << " " << stats.at<int>(i, 3) << " " << stats.at<int>(i, 4)<< " " << src.at<Vec3b>(y, x) << endl;
	}
	imshow("src", src);
	imshow("gray", gray);
	imshow("bin", bin);
	waitKey(0);
	return 0;
}

/* 과제3번
#include <opencv2/opencv.hpp> // OpenCV 라이브러리 포함
#include <iostream> // 표준 입출력 라이브러리 헤더 파일을 포함
using namespace cv; // OpenCV 네임스페이스 사용
using namespace std; // 표준 네임스페이스(std)를 사용

int main() { // 메인 함수 시작
	Mat src = imread("polygon.bmp");
	if (src.empty()) { cerr << "failed" << endl; return -1; }
	Mat gray, bin, labels, stats, centroids;
	cvtColor(src, gray, COLOR_BGR2GRAY); 
	threshold(gray, bin, 150, 255, THRESH_BINARY_INV);
	double cnt = connectedComponentsWithStats(bin, labels, stats, centroids); // 이진화된 이미지에서 연결된 객체의 수 및 각 객체의 통계 및 중심 좌표 계산
	for (int i = 1; i < cnt; i++) { // (0은 배경) 1부터 시작
		int count = 0; // 화소 개수를 저장할 변수
		double xsum = 0, ysum = 0; // 좌표 합을 저장할 변수
		for (double x = stats.at<int>(i, 0); x < stats.at<int>(i, 0) + stats.at<int>(i, 2); x++) { // 객체 영역의 x 좌표
			for (double y = stats.at<int>(i, 1); y < stats.at<int>(i, 1) + stats.at<int>(i, 3); y++) { // 객체 영역의 y 좌표
				if (bin.at<uchar>(y, x) == 255) {// 힌 픽셀 감지
					count++; // 화소 개수
					xsum += x; // x 좌표 합
					ysum += y; // y 좌표 합
				}
			}
		}
		cout << i << "번 객체의 무게중심:( " << xsum / count << ", " << ysum / count << ")" << endl;
	}
	return 0; // 프로그램 종료
} // 메인 함수 끝
*/
/*4번 과제
#include <opencv2/opencv.hpp> // OpenCV 라이브러리 포함
#include <iostream> // 표준 입출력 라이브러리 헤더 파일을 포함
using namespace cv; // OpenCV 네임스페이스 사용
using namespace std; // 표준 네임스페이스(std)를 사용
int main() {
	Mat src = imread("car.jpg");// 자동차 이미지 로드
	if (src.empty()) { cerr << "이미지를 불러오는 데 실패하였습니다!" << endl; return -1; }
	Mat gray, blurred, edges, binary, closed, labels, stats, centroids, dst;
	cvtColor(src, gray, COLOR_BGR2GRAY);// 그레이스케일 변환
	blur(gray, blurred, Size(5, 5));// 블러링 적용
	Sobel(blurred, edges, CV_8U, 1, 0, 3);// 수직 방향 에지 검출
	threshold(edges, binary, 150, 255, THRESH_BINARY);// 이진화
	Mat kernel = getStructuringElement(MORPH_RECT, Size(50, 5));
	morphologyEx(binary, closed, MORPH_CLOSE, kernel);// 닫기 연산 수행
	int cnt = connectedComponentsWithStats(closed, labels, stats, centroids);// 번호판 영역 검출
	cvtColor(src, dst, COLOR_BGR2BGRA); // 알파 채널 추가
	for (int i = 1; i < cnt; ++i) {
		int area = stats.at<int>(i, 4);
		if (area > 2000) { 
			int x = stats.at<int>(i, 0),y = stats.at<int>(i, 1),w = stats.at<int>(i, 2),h = stats.at<int>(i, 3);
			rectangle(src, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 2);
		}
	}
	imshow("원본", src);
	imshow("번호판 검출", dst);// 결과 출력
	waitKey();
	return 0;
}
*/