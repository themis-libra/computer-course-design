/*!
 * \file ConfigurationPlan.h
 * \date 2021/01/9 10:20
 *
 * \author lzk
 * Contact: 1274962676@qq.com
 *
 * \brief ���з�������ģ��
 *
 * TODO: long description
 *		��1��ʵ�����ɲ��з����㷨�ĺ�����
 * \note
*/

#pragma once
#include "pch.h"
#include <vector>
#include "Classes.h"

std::vector<Layout_Plan_End> ConfigurationPlanCreation(const std::vector<std::vector<int>>& others_car_info, std::vector<hovercraft_info> hovercrafts, std::vector<Car_Info> obstacle_cars);
