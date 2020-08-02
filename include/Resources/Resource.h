#pragma once

// all resources are owned by the resource manager and thus shall be non-copyable
class Resource {
    protected:
        virtual void release() = 0;

    public:
        Resource() = default;

        // delete copy constructor and copy assignment
        Resource(const Resource&) = delete;
        Resource &operator=(const Resource&) = delete;
};
