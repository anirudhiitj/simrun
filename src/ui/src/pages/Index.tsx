import { ComponentToolbar } from '@/components/toolbar/ComponentToolbar';
import { ArchitectureCanvas } from '@/components/canvas/ArchitectureCanvas';
import { InspectorPanel } from '@/components/inspector/InspectorPanel';
import { CanvasHeader } from '@/components/header/CanvasHeader';
import { ConfigTabs } from '@/components/panels/ConfigTabs';
import { TooltipProvider } from '@/components/ui/tooltip';
import { useArchitectureStore } from '@/store/architectureStore';
import { useSimulationStore } from '@/store/simulationStore';
import { useState } from "react";
import { toast } from "sonner";
import { serializeToIR, wrapForCompiler } from '@/lib/irSerializer';
import { ComponentCategory } from '@/types/architecture';
import { ComponentParameters, NetworkParameters, DEFAULT_DATABASE_PARAMS, DEFAULT_CACHE_PARAMS, DEFAULT_API_PARAMS, DEFAULT_NETWORK_PARAMS } from '@/types/simulation';

const Index = () => {
  const [simulationResult, setSimulationResult] = useState<any>(null);
  const { nodes, edges } = useArchitectureStore();
  const { routes, workload, faults } = useSimulationStore();

  const getDefaultParams = (category: ComponentCategory): ComponentParameters => {
    switch (category) {
      case 'database':
        return DEFAULT_DATABASE_PARAMS;
      case 'cache':
        return DEFAULT_CACHE_PARAMS;
      case 'api':
        return DEFAULT_API_PARAMS;
      case 'network':
        return DEFAULT_NETWORK_PARAMS;
      default:
        return DEFAULT_API_PARAMS;
    }
  };

  const handleRunSimulation = async () => {
    if (nodes.length === 0) {
      toast.error("Please add components to the canvas");
      return;
    }

    try {
      // Build the architecture export from canvas state
      const exportData = {
        components: nodes.map((node) => ({
          id: node.id,
          type: node.data.category as string,
          profile: node.data.profile as string,
          position: node.position,
          label: node.data.label as string,
          parameters: {
            ...getDefaultParams(node.data.category as ComponentCategory),
            ...(node.data.parameters as ComponentParameters),
          },
        })),
        links: edges.map((edge) => ({
          id: edge.id,
          source: edge.source,
          target: edge.target,
          parameters: {
            ...DEFAULT_NETWORK_PARAMS,
            ...((edge.data?.parameters as NetworkParameters) || {}),
          },
        })),
        routes,
        workload,
        faults,
        metadata: {
          version: '1.0.0',
          createdAt: new Date().toISOString(),
        },
      };

      console.log("📤 [UI] Export data:", exportData);
      console.log("📤 [UI] Components count:", exportData.components.length);

      // Serialize to IR format
      const ir = serializeToIR(exportData);
      console.log("📤 [UI] Serialized IR:", ir);
      console.log("📤 [UI] IR Components:", ir.context.components);
      
      const compilerRequest = wrapForCompiler(ir);
      console.log("📤 [UI] Final request to compiler:", compilerRequest);

      toast.promise(
        window.api.simulate(compilerRequest),
        {
          loading: 'Running simulation...',
          success: (result) => {
            console.log("📥 [UI] Response from compiler:", result);
            setSimulationResult(result);
            return 'Simulation completed 🚀';
          },
          error: (err) => {
            console.error("📥 [UI] Error from compiler:", err);
            return 'Compiler error - check console';
          }
        }
      );
    } catch (err) {
      console.error("❌ [UI] Failed to prepare simulation:", err);
      toast.error("Failed to prepare simulation");
    }
  };

  return (
    <TooltipProvider>
      <div className="flex h-screen w-full overflow-hidden bg-background">
        {/* Left Toolbar */}
        <ComponentToolbar />

        {/* Main Content Area */}
        <div className="flex-1 flex flex-col min-w-0">
          {/* Header */}
          <CanvasHeader onRunSimulation={handleRunSimulation} />

          {/* Canvas */}
          <ArchitectureCanvas />
        </div>

        {/* Right Side: Inspector + Config Tabs */}
        <div className="flex">
          <InspectorPanel />
          <ConfigTabs />
        </div>

        {/* Simulation Result Overlay */}
        {simulationResult && (
          <div className="absolute bottom-4 left-4 right-4 max-h-64 overflow-auto bg-black/80 text-white text-xs p-4 rounded-lg border border-white/20 z-50">
            <h3 className="font-bold mb-2">Simulation Output</h3>
            <pre>{JSON.stringify(simulationResult, null, 2)}</pre>
          </div>
        )}
      </div>
    </TooltipProvider>
  );
};

export default Index;
