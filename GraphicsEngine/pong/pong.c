#include "pong.h"

int 
paddle_height = 50, 
ball_rad = 10,
ball_vel_inc = 1,
xvelocity_ball = 10,
xvelocity_randrange = 1,
yvelocity_ball = 10,
yvelocity_randrange = 5,
paddle_x_pad = 5,
paddle_vel = 20,
paddle_width = 5,
paddle_color = BLACK,
ball_color = BLUE;


int left_y, right_y;
int ballvel_x, ballvel_y;
int ball_x, ball_y;


bool up_pressed = false, down_pressed = false;
bool w_pressed = false, s_pressed = false;

int up_code, down_code, w_code, s_code, W_code, S_code;

Display *dis;
XEvent evt;

int pong_init(Display *_dis)
{
	dis = _dis;

	up_code = XKeysymToKeycode(dis, XK_Up);
	down_code = XKeysymToKeycode(dis, XK_Down);

	w_code = XKeysymToKeycode(dis, XK_w);	
	W_code = XKeysymToKeycode(dis, XK_W);	

	s_code = XKeysymToKeycode(dis, XK_s);
	S_code = XKeysymToKeycode(dis, XK_S);

	left_y = HEIGHT / 2 - paddle_height / 2;
	right_y = HEIGHT / 2 - paddle_height / 2;

	ballvel_x = (rand() % 2) ? 10 : -10;
	ballvel_y = (rand() % (yvelocity_ball * 2)) - yvelocity_ball;

	ball_x = WIDTH / 2;
	ball_y = HEIGHT / 2;

	sleep (1);

	return 0;
}

int keyPressed (int keyCode)
{
	if (keyCode == up_code)
		up_pressed = true;
	else if (keyCode == down_code)
		down_pressed = true;
	else if (keyCode == w_code)
		w_pressed = true;
	else if (keyCode == W_code)
		w_pressed = true;
	else if (keyCode == s_code)
		s_pressed = true;
	else if (keyCode == S_code)
		s_pressed = true;

	return 0;
}

int keyReleased (int keyCode)
{
	if (keyCode == up_code)
		up_pressed = false;
	else if (keyCode == down_code)
		down_pressed = false;
	else if (keyCode == w_code)
		w_pressed = false;
	else if (keyCode == W_code)
		w_pressed = false;
	else if (keyCode == s_code)
		s_pressed = false;
	else if (keyCode == S_code)
		s_pressed = false;

	return 0;
}

int draw_paddles ()
{
	g2d_set_col (paddle_color);
	g2d_fill_rect (paddle_x_pad, left_y, paddle_width, paddle_height); //left paddle
	g2d_fill_rect (WIDTH - paddle_x_pad - paddle_width, right_y, paddle_width, paddle_height); //left paddle
	return 0;
}

int draw_ball ()
{
	g2d_set_col (ball_color);
	g2d_fill_ellipse (ball_x, ball_y, ball_rad, ball_rad);
	return 0;
}

int move_paddles ()
{
	if (w_pressed)
	{
		left_y -= paddle_vel;
	}
	else if (s_pressed)
	{
		left_y += paddle_vel;
	}

	if (up_pressed)
	{
		right_y -= paddle_vel;
	}
	else if (down_pressed)
	{
		right_y += paddle_vel;
	}

	if (left_y < 0) left_y = 0;
	if (left_y > HEIGHT - paddle_height) left_y = HEIGHT - paddle_height;

	if (right_y < 0) right_y = 0;
	if (right_y > HEIGHT - paddle_height) right_y = HEIGHT - paddle_height;

	return 0;
}

int move_ball()
{
	//bounce off the left side
	if (ball_x < ball_rad + paddle_x_pad)
	{
		//hit paddle, bounce off
		if (ball_y > left_y && ball_y < left_y + paddle_height)
		{
			ballvel_x *= -1;
			ballvel_x += ballvel_x > 0 ? ball_vel_inc : -ball_vel_inc;

			ballvel_x += (rand() % (2 * xvelocity_randrange)) - xvelocity_randrange;
			ballvel_y += (rand() % (2 * yvelocity_randrange)) - yvelocity_randrange;
		}

		else
		{
			pong_init (dis); //restart, we aren't keeping track of score anyways
		}
	}

	//bounce off the right side
	if (ball_x > WIDTH - ball_rad - paddle_x_pad)
	{
		//hit paddle, bounce off
		if (ball_y > right_y && ball_y < right_y + paddle_height)
		{
			ballvel_x *= -1;
			ballvel_x += ballvel_x > 0 ? ball_vel_inc : -ball_vel_inc;

			ballvel_x += (rand() % (2 * xvelocity_randrange)) - xvelocity_randrange;
			ballvel_y += (rand() % (2 * yvelocity_randrange)) - yvelocity_randrange;
		}

		else
		{
			pong_init (dis); //restart, we aren't keeping track of score anyways
		}
	}


	//bounce off the top
	if (ball_y < ball_rad)
	{
		ball_y = ball_rad;
		ballvel_y *= -1;
	}

	//bounce off the bottom
	if (ball_y > HEIGHT - ball_rad)
	{
		ball_y = HEIGHT - ball_rad;
		ballvel_y *= -1;
	}

	ball_x += ballvel_x;
	ball_y += ballvel_y;

	return 0;
}

int pong_run_frame()
{
	while(XPending(dis)) {
        XNextEvent(dis, &evt);
        if (evt.type == KeyPress)
        {
        	keyPressed (evt.xkey.keycode);
        }
        else if (evt.type == KeyRelease)
        {
        	keyReleased (evt.xkey.keycode);
        }
    }
    g2d_fill_bg (WHITE);

    move_paddles();
    move_ball();

    draw_paddles();
    draw_ball();

    put_frame();

    return 0;
}