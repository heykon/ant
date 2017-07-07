#include "ant.h"


void ant::prepare_for_next_frame()
{

	if (target == NULL)
		return;
	if (!moving)
	{
		if (target->get_pos(0) < get_pos(0))
		{
			if (test_collide(LEFT, speed))
			{
				if (target->get_pos(1) < get_pos(1))
				{
					change_anim(0);
					move_dir = UP;

					if (test_collide(UP, speed))
					{
						moving = false;
					}
					else
					{
						moving = true;
					}
				}
				else if (target->get_pos(1) > get_pos(1))
				{
					change_anim(1);
					move_dir = DOWN;

					if (test_collide(DOWN, speed))
					{
						moving = false;
					}
					else
					{
						moving = true;
					}
				}
				else
					moving = false;
			}
			else
			{
				change_anim(2);
				move_dir = LEFT;
				moving = true;
			}
		}
		else if (target->get_pos(0) > get_pos(0))
		{
			if (test_collide(RIGHT, speed))
			{
				if (target->get_pos(1) < get_pos(1))
				{
					change_anim(0);
					move_dir = UP;

					if (test_collide(UP, speed))
					{
						moving = false;
					}
					else
					{
						moving = true;
					}
				}
				else if (target->get_pos(1) > get_pos(1))
				{
					change_anim(1);
					move_dir = DOWN;

					if (test_collide(DOWN, speed))
					{
						moving = false;
					}
					else
					{
						moving = true;
					}
				}
				else
					moving = false;
			}
			else
			{
				change_anim(3);
				move_dir = RIGHT;
				moving = true;
			}
		}
		else
		{
			if (target->get_pos(1) < get_pos(1))
			{
				change_anim(0);
				move_dir = UP;

				if (test_collide(UP, speed))
				{
					moving = false;
				}
				else
				{
					moving = true;
				}
			}
			else if (target->get_pos(1) > get_pos(1))
			{
				change_anim(1);
				move_dir = DOWN;

				if (test_collide(DOWN, speed))
				{
					moving = false;
				}
				else
				{
					moving = true;
				}
			}
			else
				moving = false;				
		}
	}
	else
	{
		if (test_collide(move_dir, speed))
		{
			switch(move_dir)
			{
				case UP:
				case DOWN:
				if (target->get_pos(0) < get_pos(0))
				{
					change_anim(2);
					move_dir = LEFT;

					if (test_collide(LEFT, speed))
						moving = false;
					else
					{
						moving = true;
					}
				}
				else if (target->get_pos(0) > get_pos(0))
				{
					change_anim(3);
					move_dir = RIGHT;

					if (test_collide(RIGHT, speed))
						moving = false;
					else
					{
						moving = true;
					}
				}
				else
					moving = false;
				break;

				case LEFT:
				case RIGHT:
				if (target->get_pos(1) < get_pos(1))
				{
					change_anim(0);
					move_dir = UP;

					if (test_collide(UP, speed))
						moving = false;
					else
					{
						moving = true;
					}
				}
				else if (target->get_pos(1) > get_pos(1))
				{
					change_anim(1);
					move_dir = DOWN;

					if (test_collide(DOWN, speed))
						moving = false;
					else
					{
						moving = true;
					}
				}
				else
					moving = false;
			}
		}
		else
		{
			switch (move_dir)
			{
				case UP:
				if (target->get_pos(1) >= get_pos(1))
				{
					if (target->get_pos(0) < get_pos(0))
					{
						change_anim(2);
						move_dir = LEFT;

						if (test_collide(LEFT, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else if (target->get_pos(0) > get_pos(0))
					{
						change_anim(3);
						move_dir = RIGHT;

						if (test_collide(RIGHT, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else
						moving = false;
				}
				break;

				case DOWN:
				if (target->get_pos(1) <= get_pos(1))
				{
					if (target->get_pos(0) < get_pos(0))
					{
						change_anim(2);
						move_dir = LEFT;

						if (test_collide(LEFT, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else if (target->get_pos(0) > get_pos(0))
					{
						change_anim(3);
						move_dir = RIGHT;

						if (test_collide(RIGHT, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else
						moving = false;
				}
				break;

				case LEFT:
				if (target->get_pos(0) >= get_pos(0))
				{
					if (target->get_pos(1) < get_pos(1))
					{
						change_anim(0);
						move_dir = UP;

						if (test_collide(UP, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else if (target->get_pos(1) > get_pos(1))
					{
						change_anim(1);
						move_dir = DOWN;


						if (test_collide(DOWN, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else
						moving = false;
				}
				break;

				case RIGHT:
				if (target->get_pos(0) <= get_pos(0))
				{
					if (target->get_pos(1) < get_pos(1))
					{
						change_anim(0);
						move_dir = UP;

						if (test_collide(UP, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else if (target->get_pos(1) > get_pos(1))
					{
						change_anim(1);
						move_dir = DOWN;
						
						if (test_collide(DOWN, speed))
							moving = false;
						else
						{
							moving = true;
						}
					}
					else
						moving = false;
				}
				break;
			}
		}
	}

	if (moving)
	{
		update_anim();
		move(move_dir, speed);
	}

}