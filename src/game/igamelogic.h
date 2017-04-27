#ifndef IGAMELOGIC_H
#define IGAMELOGIC_H

#include "bizdata.h"

namespace fgame{
class IGameLogic{
public:
IGameLogic() {}
virtual ~IGameLogic() {}
virtual bool play(const BizData& biz_data, BizOutData& out) = 0;
virtual bool tick(BizOutData& out) = 0;
};

typedef std::shared_ptr<IGameLogic> IGameLogicPtr;

}


#endif
