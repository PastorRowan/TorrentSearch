
#include "providers/TorrentProvider.h"

class EZTVProvider : public TorrentProvider {

    public:

        explicit EZTVProvider(QObject* parent = nullptr);

        void search(
            const unsigned int searchId,
            const QString& query
        ) const override;

};
