#pragma once

#include "dawn/native/DawnNative.h"

namespace hub33k {

  wgpu::Instance CreateInstance();

  wgpu::Adapter CreateAdapter(const wgpu::Instance &instance, const wgpu::Surface &surface);

  wgpu::Device CreateDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter);

  wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window);

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    int width, int height, bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
    const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
  );

  // ================================================================

  wgpu::ShaderModule LoadShaderModule(const std::string &path, const wgpu::Device &device);

  // ================================================================

  // DawnInfo.cpp (dawn)
  namespace WebGPU::Info {

    // Wraps a string to about 75 characters and prints indented. Splits on whitespace instead of
    // between characters in a word.
    inline std::string WrapString(const std::string &in, const std::string &indent) {
      std::stringstream out;

      size_t last_space = 0;
      size_t start_pos = 0;
      for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == ' ' || in[i] == '\n') {
          last_space = i;
        }

        if ((i - start_pos) != 0 && ((i - start_pos) % 75) == 0) {
          out << indent << in.substr(start_pos, last_space - start_pos) << "\n";
          start_pos = last_space + 1;
          last_space = start_pos;
        }
      }
      out << indent << in.substr(start_pos, in.size() - start_pos);

      return out.str();
    }

    inline std::string AdapterTypeToString(const wgpu::AdapterType type) {
      switch (type) {
        case wgpu::AdapterType::DiscreteGPU:
          return "discrete GPU";
        case wgpu::AdapterType::IntegratedGPU:
          return "integrated GPU";
        case wgpu::AdapterType::CPU:
          return "CPU";
        case wgpu::AdapterType::Unknown:
          break;
      }
      return "unknown";
    }

    inline std::string BackendTypeToString(const wgpu::BackendType type) {
      switch (type) {
        case wgpu::BackendType::Null:
          return "Null";
        case wgpu::BackendType::WebGPU:
          return "WebGPU";
        case wgpu::BackendType::D3D11:
          return "D3D11";
        case wgpu::BackendType::D3D12:
          return "D3D12";
        case wgpu::BackendType::Metal:
          return "Metal";
        case wgpu::BackendType::Vulkan:
          return "Vulkan";
        case wgpu::BackendType::OpenGL:
          return "OpenGL";
        case wgpu::BackendType::OpenGLES:
          return "OpenGLES";
        case wgpu::BackendType::Undefined:
          return "Undefined";
      }
      return "unknown";
    }

    inline std::string PowerPreferenceToString(const wgpu::DawnAdapterPropertiesPowerPreference &prop) {
      switch (prop.powerPreference) {
        case wgpu::PowerPreference::LowPower:
          return "low power";
        case wgpu::PowerPreference::HighPerformance:
          return "high performance";
        case wgpu::PowerPreference::Undefined:
          return "<undefined>";
      }
      return "<unknown>";
    }

    inline std::string AsHex(const uint32_t val) {
      std::stringstream hex;
      hex << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << val;
      return hex.str();
    }

    inline std::string AdapterInfoToString(const wgpu::AdapterInfo &info) {
      std::stringstream out;
      out << "VendorID: " << AsHex(info.vendorID) << "\n";
      out << "Vendor: " << info.vendor.data << "\n";
      out << "Architecture: " << info.architecture.data << "\n";
      out << "DeviceID: " << AsHex(info.deviceID) << "\n";
      out << "Name: " << info.device.data << "\n";
      out << "Driver description: " << info.description.data << "\n";
      out << "Adapter Type: " << AdapterTypeToString(info.adapterType) << "\n";
      out << "Backend Type: " << BackendTypeToString(info.backendType) << "\n";

      return out.str();
    }

    inline std::string FormatNumber(const uint64_t num) {
      const auto s = std::to_string(num);
      std::stringstream ret;

      const auto remainder = s.length() % 3;
      ret << s.substr(0, remainder);
      for (size_t i = remainder; i < s.length(); i += 3) {
        if (i > 0) {
          ret << ",";
        }
        ret << s.substr(i, 3);
      }
      return ret.str();
    }

    inline std::string LimitsToString(const wgpu::Limits &limits, const std::string &indent) {
      std::stringstream out;

      out << indent << "maxTextureDimension1D: " << FormatNumber(limits.maxTextureDimension1D) << "\n";
      out << indent << "maxTextureDimension2D: " << FormatNumber(limits.maxTextureDimension2D) << "\n";
      out << indent << "maxTextureDimension3D: " << FormatNumber(limits.maxTextureDimension3D) << "\n";
      out << indent << "maxTextureArrayLayers: " << FormatNumber(limits.maxTextureArrayLayers) << "\n";
      out << indent << "maxBindGroups: " << FormatNumber(limits.maxBindGroups) << "\n";
      out << indent << "maxBindGroupsPlusVertexBuffers: " << FormatNumber(limits.maxBindGroupsPlusVertexBuffers)
          << "\n";
      out << indent << "maxBindingsPerBindGroup: " << FormatNumber(limits.maxBindingsPerBindGroup) << "\n";
      out << indent << "maxDynamicUniformBuffersPerPipelineLayout: "
          << FormatNumber(limits.maxDynamicUniformBuffersPerPipelineLayout) << "\n";
      out << indent << "maxDynamicStorageBuffersPerPipelineLayout: "
          << FormatNumber(limits.maxDynamicStorageBuffersPerPipelineLayout) << "\n";
      out << indent << "maxSampledTexturesPerShaderStage: " << FormatNumber(limits.maxSampledTexturesPerShaderStage)
          << "\n";
      out << indent << "maxSamplersPerShaderStage: " << FormatNumber(limits.maxSamplersPerShaderStage) << "\n";
      out << indent << "maxStorageBuffersPerShaderStage: " << FormatNumber(limits.maxStorageBuffersPerShaderStage)
          << "\n";
      out << indent << "maxStorageTexturesPerShaderStage: " << FormatNumber(limits.maxStorageTexturesPerShaderStage)
          << "\n";
      out << indent << "maxUniformBuffersPerShaderStage: " << FormatNumber(limits.maxUniformBuffersPerShaderStage)
          << "\n";
      out << indent << "maxUniformBufferBindingSize: " << FormatNumber(limits.maxUniformBufferBindingSize) << "\n";
      out << indent << "maxStorageBufferBindingSize: " << FormatNumber(limits.maxStorageBufferBindingSize) << "\n";
      out << indent << "minUniformBufferOffsetAlignment: " << FormatNumber(limits.minUniformBufferOffsetAlignment)
          << "\n";
      out << indent << "minStorageBufferOffsetAlignment: " << FormatNumber(limits.minStorageBufferOffsetAlignment)
          << "\n";
      out << indent << "maxVertexBuffers: " << FormatNumber(limits.maxVertexBuffers) << "\n";
      out << indent << "maxBufferSize: " << FormatNumber(limits.maxBufferSize) << "\n";
      out << indent << "maxVertexAttributes: " << FormatNumber(limits.maxVertexAttributes) << "\n";
      out << indent << "maxVertexBufferArrayStride: " << FormatNumber(limits.maxVertexBufferArrayStride) << "\n";
      out << indent << "maxInterStageShaderVariables: " << FormatNumber(limits.maxInterStageShaderVariables) << "\n";
      out << indent << "maxColorAttachments: " << FormatNumber(limits.maxColorAttachments) << "\n";
      out << indent << "maxColorAttachmentBytesPerSample: " << FormatNumber(limits.maxColorAttachmentBytesPerSample)
          << "\n";
      out << indent << "maxComputeWorkgroupStorageSize: " << FormatNumber(limits.maxComputeWorkgroupStorageSize)
          << "\n";
      out << indent << "maxComputeInvocationsPerWorkgroup: " << FormatNumber(limits.maxComputeInvocationsPerWorkgroup)
          << "\n";
      out << indent << "maxComputeWorkgroupSizeX: " << FormatNumber(limits.maxComputeWorkgroupSizeX) << "\n";
      out << indent << "maxComputeWorkgroupSizeY: " << FormatNumber(limits.maxComputeWorkgroupSizeY) << "\n";
      out << indent << "maxComputeWorkgroupSizeZ: " << FormatNumber(limits.maxComputeWorkgroupSizeZ) << "\n";
      out << indent << "maxComputeWorkgroupsPerDimension: " << FormatNumber(limits.maxComputeWorkgroupsPerDimension)
          << "\n";

      return out.str();
    }

    // ================================================================

    inline void DumpAdapterInfo(const wgpu::Adapter &adapter) {
      wgpu::AdapterPropertiesSubgroups subgroup_props{};

      wgpu::DawnAdapterPropertiesPowerPreference power_props{};
      power_props.nextInChain = &subgroup_props;

      wgpu::AdapterInfo info{};
      info.nextInChain = &power_props;

      adapter.GetInfo(&info);
      std::cout << AdapterInfoToString(info);
      std::cout << "Subgroup min size: " << subgroup_props.subgroupMinSize << "\n";
      std::cout << "Subgroup max size: " << subgroup_props.subgroupMaxSize << "\n";
      std::cout << "Power: " << PowerPreferenceToString(power_props) << "\n";
      std::cout << "\n";
    }

    inline void DumpAdapterFeatures(const wgpu::Adapter &adapter) {
      wgpu::SupportedFeatures supportedFeatures;
      adapter.GetFeatures(&supportedFeatures);
      std::cout << "  Features\n";
      std::cout << "  ========\n";
      for (uint32_t i = 0; i < supportedFeatures.featureCount; ++i) {
        const wgpu::FeatureName f = supportedFeatures.features[i];
        const auto info = dawn::native::GetFeatureInfo(f);
        std::cout << "   * " << info->name << "\n";
        std::cout << WrapString(info->description, "      ") << "\n";
        std::cout << "      " << info->url << "\n";
      }
    }

    inline void DumpAdapterLimits(const wgpu::Adapter &adapter) {
      wgpu::Limits adapterLimits;
      if (adapter.GetLimits(&adapterLimits)) {
        std::cout << "\n";
        std::cout << "  Adapter Limits\n";
        std::cout << "  ==============\n";
        std::cout << LimitsToString(adapterLimits, "    ") << "\n";
      }
    }

    inline void DumpAdapter(const wgpu::Adapter &adapter) {
      std::cout << "Adapter\n";
      std::cout << "=======\n";

      DumpAdapterInfo(adapter);
      DumpAdapterFeatures(adapter);
      DumpAdapterLimits(adapter);
    }

    // ================================================================

    inline void DumpDeviceFeatures(const wgpu::Device &device) {
      wgpu::SupportedFeatures supportedFeatures;
      device.GetFeatures(&supportedFeatures);
      std::cout << "  Device Features\n";
      std::cout << "  ========\n";
      for (uint32_t i = 0; i < supportedFeatures.featureCount; ++i) {
        const wgpu::FeatureName f = supportedFeatures.features[i];
        const auto info = dawn::native::GetFeatureInfo(f);
        std::cout << "   * " << info->name << "\n";
        std::cout << WrapString(info->description, "      ") << "\n";
        std::cout << "      " << info->url << "\n";
      }
    }

    inline void DumpDeviceLimits(const wgpu::Device &device) {
      wgpu::Limits deviceLimits;
      if (device.GetLimits(&deviceLimits)) {
        std::cout << "\n";
        std::cout << "  Device Limits\n";
        std::cout << "  ==============\n";
        std::cout << LimitsToString(deviceLimits, "    ") << "\n";
      }
    }

    inline void DumpDevice(const wgpu::Device &device) {
      std::cout << "\n";
      std::cout << "Device\n";
      std::cout << "======\n";

      DumpDeviceFeatures(device);
      DumpDeviceLimits(device);
    }

  } // namespace WebGPU::Info

} // namespace hub33k
