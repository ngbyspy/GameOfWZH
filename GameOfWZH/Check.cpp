#include "Check.h"
#include <cmath>
#include <iostream>

//���´������ڼ��һ�������κ�һ�������Ƿ��ཻ--------------------------------------------------------------------------------------------------------------------
// ���������νṹ��


// ������������Ƿ���ײ
int checkRectangleCollision(MyRectangle rect1, MyRectangle rect2) {
    if (rect1.x < rect2.x + rect2.width &&
        rect1.x + rect1.width > rect2.x &&
        rect1.y < rect2.y + rect2.height &&
        rect1.y + rect1.height > rect2.y) {
        return 1; // ����������ײ
    }
    return 0; // �������β���ײ
}

// ���������������Ƿ���ײ
int checkTriangleRectangleCollision(MyTriangle triangle, MyRectangle rect) {
    // ��������ε�ÿһ����������Ƿ��ཻ
    if (checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y, rect.x, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y, rect.x, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y, rect.x, rect.y + rect.height)) {
        return 1; // ������������ཻ
    }
    return 0; // ����������β��ཻ
}

// ��������߶��Ƿ��ཻ
int checkLineSegmentsIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int d1 = direction(x3, y3, x4, y4, x1, y1);
    int d2 = direction(x3, y3, x4, y4, x2, y2);
    int d3 = direction(x1, y1, x2, y2, x3, y3);
    int d4 = direction(x1, y1, x2, y2, x4, y4);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return 1; // �����߶��ཻ
    }
    else if (d1 == 0 && onSegment(x3, y3, x4, y4, x1, y1)) {
        return 1; // �����߶ι��������ص�����
    }
    else if (d2 == 0 && onSegment(x3, y3, x4, y4, x2, y2)) {
        return 1; // �����߶ι��������ص�����
    }
    else if (d3 == 0 && onSegment(x1, y1, x2, y2, x3, y3)) {
        return 1; // �����߶ι��������ص�����
    }
    else if (d4 == 0 && onSegment(x1, y1, x2, y2, x4, y4)) {
        return 1; // �����߶ι��������ص�����
    }
    return 0; // �����߶β��ཻ
}

// ���������ķ���
int direction(int x1, int y1, int x2, int y2, int x3, int y3) {
    return ((x2 - x1) * (y3 - y1)) - ((x3 - x1) * (y2 - y1));
}

// �жϵ��Ƿ����߶���
int onSegment(int x1, int y1, int x2, int y2, int x, int y) {
    if (x >= minNum(x1, x2) && x <= maxNum(x1, x2) && y >= minNum(y1, y2) && y <= maxNum(y1, y2)) {
        return 1; // �����߶���
    }
    return 0; // �㲻���߶���
}

// ��ȡ�������е���Сֵ
int minNum(int a, int b) {
    return a < b ? a : b;
}

// ��ȡ�������е����ֵ
int maxNum(int a, int b) {
    return a > b ? a : b;
}

//���´������ڼ��������Բ�Ƿ��ཻ--------------------------------------------------------------------------------------------------------------------

// ������Բ�ṹ��
//struct Ellipse {
//    float x, y;     // ��Բ��������
//    float width, height;   // ��Բ�Ŀ�Ⱥ͸߶�
//};
//
//// ���������Բ�Ƿ���ײ
//bool checkEllipseCollision(const Ellipse& ellipse1, const Ellipse& ellipse2) {
//    float dx = ellipse1.x - ellipse2.x;
//    float dy = ellipse1.y - ellipse2.y;
//    float distance = std::sqrt(dx * dx + dy * dy);
//    return distance <= (ellipse1.width / 2 + ellipse2.width / 2);
//}
//���ϴ������ڼ��������Բ�Ƿ��ཻ--------------------------------------------------------------------------------------------------------------------