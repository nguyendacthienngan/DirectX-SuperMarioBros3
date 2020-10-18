#pragma once
#include "KeyEventHandler.h"

// implement lại KeyEventHandler. KeyEventHandler chỉ là một bản mẫu cha của tất cả
// Game Key Event Handler sẽ chỉ là ảo chứ không thuần ảo. Chỉ khi các object cần implement lại thì tự override lại
// GameKeyEventHandler sẽ bao gồm 2 hàm OnKeyDown và OnKeyUp (bỏ đi hàm KeyState vì ta sẽ implement thẳng trong Update của Game)
// Ta sẽ truyền các tham số keyCode xuống cho các hàm OnKeyDown và OnKeyUp của mỗi object thông qua mỗi scene
// Mỗi object sẽ tự xử lý bàn phím cho riêng nó
// Chứ scene không cần phải tự xử lý

// Nhưng có vấn đề là những nút chung cho cả game thì sẽ handle như thế nào cho tổng quát ?

class CGameKeyEventHandler : public CKeyEventHandler
{
public:
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	virtual void KeyState();
};

